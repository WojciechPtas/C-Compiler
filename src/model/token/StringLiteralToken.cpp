#include "StringLiteralToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

StringLiteralToken::StringLiteralToken(TokenPosition position, string value)
    : Token(position), value(value) { }

void StringLiteralToken::accept(ITokenVisitor &visitor) const {
    visitor.visit(*this);
}

const string StringLiteralToken::getValue() const {
    return this->value;
}
