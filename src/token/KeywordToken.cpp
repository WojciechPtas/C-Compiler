#include "KeywordToken.h"

using namespace c4::token;

KeywordToken::KeywordToken(TokenPosition position, Keyword keyword)
    : Token(position) {
    this->keyword = keyword;
}

void KeywordToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

Keyword KeywordToken::getKeyword() {
    return this->keyword;
}
