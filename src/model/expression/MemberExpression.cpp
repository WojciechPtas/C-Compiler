#include "MemberExpression.h"

using namespace c4::model::expression;

MemberExpression::MemberExpression(
    MemberExpressionType type,
    const IExpression &container,
    const IdentifierExpression &member
) : container(container), member(member), type(type) { }

const IExpression &MemberExpression::getContainer() const {
    return this->container;
}

const IdentifierExpression &MemberExpression::getMember() const {
    return this->member;
}

MemberExpressionType MemberExpression::getType() const {
    return this->type;
}
