#include "StringLiteralToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

StringLiteralToken::StringLiteralToken(TokenPosition position, string value)
    : Token(position) {
    this->value = value;
}

void StringLiteralToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

string StringLiteralToken::getValue() {
    return this->value;
}
