#include "BinaryExpression.h"

using namespace c4::model::expression;
using namespace std;

BinaryExpression::BinaryExpression(
    BinaryExpressionType type,
    const shared_ptr<const IExpression> left,
    const shared_ptr<const IExpression> right
) : left(left), right(right), type(type) { }

shared_ptr<const IExpression> BinaryExpression::getLeft() const {
    return this->left;
}

shared_ptr<const IExpression> BinaryExpression::getRight() const {
    return this->right;
}

BinaryExpressionType BinaryExpression::getType() const {
    return this->type;
}
