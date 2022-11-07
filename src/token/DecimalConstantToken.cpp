#include "DecimalConstantToken.h"

using namespace c4::token;
using namespace std;

DecimalConstantToken::DecimalConstantToken(string value) {
    this->value = value;
}

void DecimalConstantToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

string DecimalConstantToken::getValue() {
    return this->value;
}
