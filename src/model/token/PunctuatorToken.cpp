#include "PunctuatorToken.h"

using namespace c4::model::token;
using namespace c4::util::token;

PunctuatorToken::PunctuatorToken(TokenPosition position, Punctuator punctuator)
    : Token(position) {
    this->punctuator = punctuator;
}

void PunctuatorToken::accept(ITokenVisitor &visitor) const {
    visitor.visit(*this);
}

Punctuator PunctuatorToken::getPunctuator() const {
    return this->punctuator;
}
