#include "UnaryExpression.h"

using namespace c4::model::expression;

UnaryExpression::UnaryExpression(const IExpression &expression) :
    expression(expression) { }

const IExpression &UnaryExpression::getExpression() const {
    return this->expression;
}
