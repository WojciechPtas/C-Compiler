#include "PunctuatorToken.h"

using namespace c4::token;

PunctuatorToken::PunctuatorToken(TokenPosition position, Punctuator punctuator)
    : Token(position) {
    this->punctuator = punctuator;
}

void PunctuatorToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

Punctuator PunctuatorToken::getPunctuator() {
    return this->punctuator;
}
