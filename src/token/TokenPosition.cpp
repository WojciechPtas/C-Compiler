#include "TokenPosition.h"

using namespace c4::token;
using namespace std;

TokenPosition::TokenPosition(string file, uint32_t line, uint32_t column) {
    this->file = file;
    this->line = line;
    this->column = column;
}

string TokenPosition::getFile() {
    return this->file;
}

uint32_t TokenPosition::getLine() {
    return this->line;
}

uint32_t TokenPosition::getColumn() {
    return this->column;
}
