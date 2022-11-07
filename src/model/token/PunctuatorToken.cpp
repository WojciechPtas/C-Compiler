#include "PunctuatorToken.h"

using namespace c4::model::token;
using namespace c4::util::token;

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
