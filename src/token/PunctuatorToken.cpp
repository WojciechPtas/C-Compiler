#include "PunctuatorToken.h"

using namespace c4::token;

PunctuatorToken::PunctuatorToken(Punctuator punctuator) {
    this->punctuator = punctuator;
}

void PunctuatorToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

Punctuator PunctuatorToken::getPunctuator() {
    return this->punctuator;
}
