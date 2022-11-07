#include "PunctuatorToken.h"

using namespace c4::token;

PunctuatorToken::PunctuatorToken(Punctuator punctuator) {
    this->punctuator = punctuator;
}

Punctuator PunctuatorToken::getPunctuator() {
    return this->punctuator;
}
