#include "UnaryExpression.h"

using namespace c4::model::expression;
using namespace std;

UnaryExpression::UnaryExpression(
    UnaryExpressionType type,
    const shared_ptr<const IExpression> expression
) : expression(expression), type(type) { }

shared_ptr<const IExpression> UnaryExpression::getExpression() const {
    return this->expression;
}

UnaryExpressionType UnaryExpression::getType() const {
    return this->type;
}
