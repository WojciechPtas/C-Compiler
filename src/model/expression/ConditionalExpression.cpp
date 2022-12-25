#include "ConditionalExpression.h"

using namespace c4::model::expression;
using namespace std;

ConditionalExpression::ConditionalExpression(
    const shared_ptr<const IExpression> condition,
    const std::shared_ptr<const IExpression> thenCase,
    const std::shared_ptr<const IExpression> elseCase
) : condition(condition), elseCase(elseCase), thenCase(thenCase) { }

std::shared_ptr<const IExpression> ConditionalExpression::getCondition() const {
    return this->condition;
}

std::shared_ptr<const IExpression> ConditionalExpression::getElseCase() const {
    return this->elseCase;
}

std::shared_ptr<const IExpression> ConditionalExpression::getThenCase() const {
    return this->thenCase;
}
