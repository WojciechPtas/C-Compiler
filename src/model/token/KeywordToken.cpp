#include "KeywordToken.h"

using namespace c4::model::token;
using namespace c4::util::token;

KeywordToken::KeywordToken(TokenPosition position, Keyword keyword)
    : Token(position) {
    this->keyword = keyword;
}

void KeywordToken::accept(ITokenVisitor &visitor) const {
    visitor.visit(*this);
}

Keyword KeywordToken::getKeyword() const {
    return this->keyword;
}
