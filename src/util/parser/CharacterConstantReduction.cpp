#include "../../model/expression/CharacterConstantExpression.h"
#include "CharacterConstantReduction.h"

using namespace c4::model::expression;
using namespace c4::model::token;
using namespace c4::util::parser;
using namespace std;

CharacterConstantReduction::CharacterConstantReduction() : TokenReduction() { }

shared_ptr<const IExpression> CharacterConstantReduction::reduce(
    shared_ptr<const CharacterConstantToken> token
) const {
    return make_shared<CharacterConstantExpression>(token->getValue());
}
