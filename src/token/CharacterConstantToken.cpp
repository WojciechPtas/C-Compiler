#include "CharacterConstantToken.h"

using namespace c4::token;
using namespace std;

CharacterConstantToken::CharacterConstantToken(string value) {
    this->value = value;
}

void CharacterConstantToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

string CharacterConstantToken::getValue() {
    return this->value;
}
