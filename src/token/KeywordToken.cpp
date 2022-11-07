#include "KeywordToken.h"

using namespace c4::token;

KeywordToken::KeywordToken(Keyword keyword) {
    this->keyword = keyword;
}

Keyword KeywordToken::getKeyword() {
    return this->keyword;
}
