#include "DecimalConstantToken.h"

using namespace c4::token;
using namespace std;

DecimalConstantToken::DecimalConstantToken(string value) {
    this->value = value;
}

string DecimalConstantToken::getValue() {
    return this->value;
}
