#include "IdentifierToken.h"

using namespace c4::token;
using namespace std;

IdentifierToken::IdentifierToken(string identifier) {
    this->identifier = identifier;
}

string IdentifierToken::getIdentifier() {
    return this->identifier;
}
