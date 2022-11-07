#include "IdentifierToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
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
