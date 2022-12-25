#include "../../model/expression/StringLiteralExpression.h"
#include "StringLiteralReduction.h"

using namespace c4::model::expression;
using namespace c4::model::token;
using namespace c4::util::parser;
using namespace std;

StringLiteralReduction::StringLiteralReduction() : TokenReduction() { }

shared_ptr<const IExpression> StringLiteralReduction::reduce(
    shared_ptr<const StringLiteralToken> token
) const {
    return make_shared<StringLiteralExpression>(token->getValue());
}
