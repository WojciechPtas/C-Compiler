#include "StringLiteralToken.h"

using namespace c4::token;
using namespace std;

StringLiteralToken::StringLiteralToken(string value) {
    this->value = value;
}

string StringLiteralToken::getValue() {
    return this->value;
}
