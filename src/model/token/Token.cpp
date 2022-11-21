#include "Token.h"

using namespace c4::model::token;

Token::Token(TokenPosition position) : position(position) { }

const TokenPosition &Token::getPosition() const {
    return this->position;
}
