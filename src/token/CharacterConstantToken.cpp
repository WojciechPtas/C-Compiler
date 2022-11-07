#include "CharacterConstantToken.h"

using namespace c4::token;
using namespace std;

CharacterConstantToken::CharacterConstantToken(
    TokenPosition position,
    string value
) : ConstantToken(position), value(value) {
}

void CharacterConstantToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

const string &CharacterConstantToken::getValue() {
    return this->value;
}
