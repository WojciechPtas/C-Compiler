#include "Token.h"

using namespace c4::token;

Token::Token(TokenPosition position) : position(position) { }

const TokenPosition &Token::getPosition() {
    return this->position;
}
