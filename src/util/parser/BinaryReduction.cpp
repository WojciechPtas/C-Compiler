#include "../../model/expression/BinaryExpression.h"
#include "BinaryReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

BinaryReduction::BinaryReduction(model::expression::BinaryExpressionType type) :
    PureReduction(3, 2), type(type) { }

shared_ptr<const IExpression> BinaryReduction::reduce(
    const vector<shared_ptr<const IExpression>> &consumedExpressions
) const {
    auto left = consumedExpressions[0];
    auto right = consumedExpressions[1];

    return make_shared<BinaryExpression>(this->type, left, right);
}
