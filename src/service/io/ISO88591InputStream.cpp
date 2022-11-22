#include <cstring>
#include <iostream>

#include "../../debug.h"
#include "ISO88591InputStream.h"

using namespace c4::service::io;
using namespace std;

#define INITIAL_BUFFER_SIZE 64

ISO88591InputStream::ISO88591InputStream(string filePath)
    : filePath(filePath), stream(filePath) {
    if (stream.fail()) {
        throw std::invalid_argument("filePath");
    }

    this->buffer = NULL;
    this->bufferCapacity = 0;
    this->bufferLimit = 0;
    this->bufferOffset = 0;

    // Metric

    this->currentLine = 1;
    this->currentColumn = 1;

    // New-line recognition

    this->lastCharWasCR = false;
}

ISO88591InputStream::~ISO88591InputStream() {
    if (this->buffer != NULL) {
        free(this->buffer);
        this->buffer = NULL;
    }
}

uint32_t ISO88591InputStream::getCurrentColumn() const {
    return this->currentColumn;
}

uint32_t ISO88591InputStream::getCurrentLine() const {
    return this->currentLine;
}

const string &ISO88591InputStream::getFilePath() const {
    return this->filePath;
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

        DBGOUT_E(
            "stream",
            "Shrinked down buffer (capacity = %d, limit = %d)",
            this->bufferCapacity,
            this->bufferLimit
        );
    }
}

void ISO88591InputStream::pushMark() {
    auto mark = make_shared<ISO88591InputStreamMark>();

    mark->bufferOffset = this->bufferOffset;
    mark->currentColumn = this->currentColumn;
    mark->currentLine = this->currentLine;
    mark->lastCharWasCR = this->lastCharWasCR;

    this->markStack.push_back(mark);
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

        DBGOUT("stream", "Released buffer");
    }

    // If there is no buffer and there is no mark, we can directly read from
    // the stream. Tracking the content is unnecessary in that case, because
    // the caller cannot move back the read cursor.

    if (this->buffer == NULL && this->markStack.empty()) {
        this->stream.read(dst, 1);
        auto success = !this->stream.eof();

        if (success) {
            this->updateMetrics(*dst);
        }

        return success;
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

        DBGOUT_E(
            "stream",
            "Allocated buffer (capacity = %d)",
            this->bufferCapacity
        );
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

            DBGOUT_E(
                "stream",
                "Enlarged buffer (capacity = %d)",
                this->bufferCapacity
            );
        }

        this->stream.read(this->buffer + this->bufferOffset, this->bufferCapacity - this->bufferLimit);
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
    this->updateMetrics(*dst);

    return true;
}

void ISO88591InputStream::resetToMark() {
    if (this->markStack.empty()) {
        throw logic_error("No mark is left.");
    }

    auto mark = this->markStack.back();

    this->bufferOffset = mark->bufferOffset;
    this->currentColumn = mark->currentColumn;
    this->currentLine = mark->currentLine;
    this->lastCharWasCR = mark->lastCharWasCR;
}

void ISO88591InputStream::updateMetrics(char readCharacter) {
    if (
        readCharacter == '\r' ||
        (readCharacter == '\n' && !this->lastCharWasCR)
    ) {
        this->currentColumn = 1;
        this->currentLine++;
    } else if (readCharacter != '\n') {
        this->currentColumn++;
    }
    
    this->lastCharWasCR = (readCharacter == '\r');
}
