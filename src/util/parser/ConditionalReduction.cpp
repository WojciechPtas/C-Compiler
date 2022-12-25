#include "../../model/expression/ConditionalExpression.h"
#include "ConditionalReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

ConditionalReduction::ConditionalReduction() : PureReduction(5, 3) { }

shared_ptr<const IExpression> ConditionalReduction::reduce(
    const vector<shared_ptr<const IExpression>> &consumedExpressions
) const {
    auto condition = consumedExpressions[0];
    auto thenCase = consumedExpressions[1];
    auto elseCase = consumedExpressions[2];

    return make_shared<ConditionalExpression>(condition, thenCase, elseCase);
}
