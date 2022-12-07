#include "IndexExpression.h"

using namespace c4::model::expression;

IndexExpression::IndexExpression(
    const IExpression &container,
    const IExpression &index
) : container(container), index(index) { }

const IExpression &IndexExpression::getContainer() const {
    return this->container;
}

const IExpression &IndexExpression::getIndex() const {
    return this->index;
}
