#include "../../model/expression/DecimalConstantExpression.h"
#include "DecimalConstantReduction.h"

using namespace c4::model::expression;
using namespace c4::model::token;
using namespace c4::util::parser;
using namespace std;

DecimalConstantReduction::DecimalConstantReduction() : TokenReduction() { }

shared_ptr<const IExpression> DecimalConstantReduction::reduce(
    shared_ptr<const DecimalConstantToken> token
) const {
    return make_shared<DecimalConstantExpression>(token->getValue());
}
