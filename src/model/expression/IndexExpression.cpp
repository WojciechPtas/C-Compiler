#include "IndexExpression.h"

using namespace c4::model::expression;
using namespace std;

IndexExpression::IndexExpression(
    shared_ptr<const IExpression> container,
    shared_ptr<const IExpression> index
) : container(container), index(index) { }

shared_ptr<const IExpression> IndexExpression::getContainer() const {
    return this->container;
}

shared_ptr<const IExpression> IndexExpression::getIndex() const {
    return this->index;
}
