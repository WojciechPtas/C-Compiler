#include "UnaryExpression.h"

using namespace c4::model::expression;

UnaryExpression::UnaryExpression(
    UnaryExpressionType type,
    const IExpression &expression
) : expression(expression), type(type) { }

const IExpression &UnaryExpression::getExpression() const {
    return this->expression;
}

const UnaryExpressionType UnaryExpression::getType() const {
    return this->type;
}
