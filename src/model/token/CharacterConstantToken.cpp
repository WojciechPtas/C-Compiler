#include "CharacterConstantToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

CharacterConstantToken::CharacterConstantToken(
    TokenPosition position,
    string value
) : ConstantToken(position), value(value) { }

void CharacterConstantToken::accept(ITokenVisitor &visitor) const {
    visitor.visit(*this);
}

const string &CharacterConstantToken::getValue() const {
    return this->value;
}
