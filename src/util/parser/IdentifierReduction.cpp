#include "../../model/expression/IdentifierExpression.h"
#include "IdentifierReduction.h"

using namespace c4::model::expression;
using namespace c4::model::token;
using namespace c4::util::parser;
using namespace std;

IdentifierReduction::IdentifierReduction() : TokenReduction() { }

shared_ptr<const IExpression> IdentifierReduction::reduce(
    shared_ptr<const IdentifierToken> token
) const {
    return make_shared<IdentifierExpression>(token->getIdentifier());
}
