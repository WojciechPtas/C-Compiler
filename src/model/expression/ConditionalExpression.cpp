#include "ConditionalExpression.h"

using namespace c4::model::expression;

ConditionalExpression::ConditionalExpression(
    const IExpression &condition,
    const IExpression &thenCase,
    const IExpression &elseCase
) : condition(condition), elseCase(elseCase), thenCase(thenCase) { }

const IExpression &ConditionalExpression::getCondition() const {
    return this->condition;
}

const IExpression &ConditionalExpression::getElseCase() const {
    return this->elseCase;
}

const IExpression &ConditionalExpression::getThenCase() const {
    return this->thenCase;
}
