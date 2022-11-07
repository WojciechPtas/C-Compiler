#include "DecimalConstantToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
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
