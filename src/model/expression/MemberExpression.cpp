#include "MemberExpression.h"

using namespace c4::model::expression;

MemberExpression::MemberExpression(
    DereferencingType accessType,
    const IExpression &container,
    const IdentifierExpression &member
) : accessType(accessType), container(container), member(member) { }

DereferencingType MemberExpression::getAccessType() const {
    return this->accessType;
}

const IExpression &MemberExpression::getContainer() const {
    return this->container;
}

const IdentifierExpression &MemberExpression::getMember() const {
    return this->member;
}
