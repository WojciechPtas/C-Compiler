#include "DecimalConstantToken.h"

using namespace c4::token;
using namespace std;

DecimalConstantToken::DecimalConstantToken(
    TokenPosition position,
    string value
) : ConstantToken(position), value(value) {
}

void DecimalConstantToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

string &DecimalConstantToken::getValue() {
    return this->value;
}
