#include "MemberExpression.h"

using namespace c4::model::expression;

MemberExpression::MemberExpression(
    const IExpression &container,
    const IdentifierExpression &member
) : container(container), member(member) { }

const IExpression &MemberExpression::getContainer() const {
    return this->container;
}

const IdentifierExpression &MemberExpression::getMember() const {
    return this->member;
}
