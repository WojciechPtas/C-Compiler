#include "IdentifierToken.h"

using namespace c4::token;
using namespace std;

IdentifierToken::IdentifierToken(TokenPosition position, string identifier)
    : Token(position), identifier(identifier) {
}

void IdentifierToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

string &IdentifierToken::getIdentifier() {
    return this->identifier;
}
