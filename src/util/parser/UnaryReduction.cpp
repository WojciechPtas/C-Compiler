#include "../../model/expression/UnaryExpression.h"
#include "UnaryReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

UnaryReduction::UnaryReduction(UnaryExpressionType type) :
    PureReduction(2, 1), type(type) { }

shared_ptr<const IExpression> UnaryReduction::reduce(
    const vector<shared_ptr<const IExpression>> &consumedExpressions
) const {
    auto expression = consumedExpressions[0];

    return make_shared<UnaryExpression>(this->type, expression);
}
