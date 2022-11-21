#include <iostream>
#include "ISO88591InputStream.h"

using namespace c4::service::io;
using namespace std;

#define INITIAL_BUFFER_SIZE 64

ISO88591InputStream::ISO88591InputStream(string filePath) : stream(filePath), filePath(filePath) {
    if (stream.fail()) {
        throw std::invalid_argument("filePath");
    }

    this->buffer = NULL;
    this->bufferCapacity = 0;
    this->bufferLimit = 0;
    this->bufferOffset = 0;
}

ISO88591InputStream::~ISO88591InputStream() {
    if (this->buffer != NULL) {
        free(this->buffer);
        this->buffer = NULL;
    }
}

void ISO88591InputStream::popMark() {
    if (this->markStack.empty()) {
        throw logic_error("No mark is left.");
    }

    this->markStack.pop_back();

    // Shrink down the buffer size by removing the leading characters, if there
    // is no mark left.

    if (this->markStack.empty() && this->bufferOffset > 0) {

        size_t remainingChars = this->bufferLimit - this->bufferOffset;

        memmove(
            this->buffer,
            this->buffer + this->bufferOffset,
            remainingChars
        );

        void *newBuffer = realloc(this->buffer, remainingChars);

        if (remainingChars > 0 && newBuffer == NULL) {
            throw bad_alloc();
        }

        this->buffer = (char *) newBuffer;
        this->bufferCapacity -= this->bufferOffset;
        this->bufferLimit = remainingChars;
        this->bufferOffset = 0;

        
    }
    std::cerr << "--- Marks present: " << markStack.size() << "\n";
}

void ISO88591InputStream::pushMark() {
    this->markStack.push_back(this->bufferOffset);
    std::cerr << "--- Marks present: " << markStack.size() << "\n";
}

bool ISO88591InputStream::read(char *dst) {
    // First, we clean up any allocated buffer that is not required anymore.
    // This is the case when there is no mark, buffer is not null, but
    // bufferOffset is equal to bufferLimit.

    if (
        this->markStack.empty() &&
        this->buffer != NULL &&
        this->bufferOffset == this->bufferLimit
    ) {
        free(this->buffer);

        this->buffer = NULL;
        this->bufferCapacity = 0;
        this->bufferLimit = 0;
        this->bufferOffset = 0;
    }

    // If there is no buffer and there is no mark, we can directly read from
    // the stream. Tracking the content is unnecessary in that case, because
    // the caller cannot move back the read cursor.

    if (this->buffer == NULL && this->markStack.empty()) {
        this->stream.read(dst, 1);
        return !this->stream.eof();
    }

    // If the markStack is not empty, but there is no buffer, we have the
    // situation that at least one mark was pushed, but nothing has been read
    // since then. Hence, we need to initialize a buffer, where we can store
    // the data for future reset operations.

    if (this->buffer == NULL) {
        this->buffer = (char *) malloc(INITIAL_BUFFER_SIZE);

        if (this->buffer == NULL) {
            throw bad_alloc();
        }

        this->bufferCapacity = INITIAL_BUFFER_SIZE;
    }

    // If we reach this point, there is a buffer that we need to consider when
    // reading something. Hence, we ensure that there is something inside the
    // buffer, that we can read, and read from there.

    if (this->bufferOffset == this->bufferLimit) {
        // We need to read more data from the underlying stream into the
        // buffer. In order to be able to this safely, we also need to ensure
        // that the total capacity of the allocated buffer is large enough.

        if (this->bufferCapacity == this->bufferLimit) {
            void *newBuffer = realloc(this->buffer, this->bufferCapacity * 2);

            if (newBuffer == NULL) {
                throw bad_alloc();
            }

            this->buffer = (char *) newBuffer;
            this->bufferCapacity *= 2;
        }

        this->stream.read(this->buffer, this->bufferCapacity - this->bufferLimit);
        this->bufferLimit += this->stream.gcount();

        // If the limit did not change, we hit EOF.

        if (this->bufferOffset == this->bufferLimit) {
            return false;
        }
    }

    // If we reach this point, bufferOffset < bufferLimit, and hence we can
    // read some character.

    *dst = this->buffer[this->bufferOffset];
    this->bufferOffset++;
    return true;
}

void ISO88591InputStream::resetToMark() {
    if (this->markStack.empty()) {
        throw logic_error("No mark is left.");
    }

    this->bufferOffset = this->markStack.back();
}
