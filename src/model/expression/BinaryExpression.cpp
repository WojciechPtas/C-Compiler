#include "BinaryExpression.h"

using namespace c4::model::expression;

BinaryExpression::BinaryExpression(
                    BinaryExpressionType type,
    const IExpression &left,
    const IExpression &right
) : left(left), right(right), type(type) { }

const IExpression &BinaryExpression::getLeft() const {
    return this->left;
}

const IExpression &BinaryExpression::getRight() const {
    return this->right;
}

BinaryExpressionType BinaryExpression::getType() const {
    return this->type;
}
