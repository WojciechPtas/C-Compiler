#include "CharacterConstantToken.h"

using namespace c4::token;
using namespace std;

CharacterConstantToken::CharacterConstantToken(string value) {
    this->value = value;
}

string CharacterConstantToken::getValue() {
    return this->value;
}
