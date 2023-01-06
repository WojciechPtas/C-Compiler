#include "MetricInputStream.h"

using namespace c4::service::io;
using namespace c4::util;
using namespace std;

MetricInputStream::MetricInputStream(
    const shared_ptr<IBufferedInputStream<char>> &src,
    const string_view &filePath
) : filePath(filePath),
    source(src),
    lastCharWasCR(false),
    lastColumn(),
    lastLine(),
    nextColumn(1),
    nextLine(1) { }

size_t MetricInputStream::getColumn() const {
    return this->nextColumn;
}

bool MetricInputStream::getLastColumn(size_t *dst) const {
    if (this->lastColumn.isEmpty) {
        return false;
    }

    *dst = this->lastColumn.value;
    return true;
}

bool MetricInputStream::getLastLine(size_t *dst) const {
    if (this->lastLine.isEmpty) {
        return false;
    }

    *dst = this->lastLine.value;
    return true;
}

size_t MetricInputStream::getLine() const {
    return this->nextLine;
}

bool MetricInputStream::popMark() {
    if (!this->source->popMark()) {
        return false;
    }

    this->markedMetrics.pop_back();
    return true;
}

void MetricInputStream::pushMark() {
    this->source->pushMark();
    this->markedMetrics.push_back({
        this->nextColumn,
        this->lastCharWasCR,
        this->lastColumn,
        this->lastLine,
        this->nextLine,
    });
}

bool MetricInputStream::read(char *dst) {
    if (!this->source->read(dst)) {
        return false;
    }

    this->lastColumn = this->nextColumn;
    this->lastLine = this->nextLine;

    if (*dst == '\r' || (*dst == '\n' && !this->lastCharWasCR)) {
        this->nextColumn = 1;
        this->nextLine++;
    } else if (*dst != '\n') {
        this->nextColumn++;
    }

    this->lastCharWasCR = *dst == '\r';
    return true;
}

bool MetricInputStream::read(char *dst, size_t *line, size_t *column) {
    *line = this->nextLine;
    *column = this->nextColumn;

    return this->read(dst);
}

bool MetricInputStream::resetToMark() {
    if (!this->source->resetToMark()) {
        return false;
    }

    auto snapshot = this->markedMetrics.back();

    this->nextColumn = snapshot.column;
    this->lastCharWasCR = snapshot.lastCharWasCR;
    this->nextLine = snapshot.line;

    return true;
}
