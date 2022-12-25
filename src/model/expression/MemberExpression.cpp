#include "MemberExpression.h"

using namespace c4::model::expression;
using namespace std;

MemberExpression::MemberExpression(
    MemberExpressionType type,
    const shared_ptr<const IExpression> container,
    const shared_ptr<const IdentifierExpression> member
) : container(container), member(member), type(type) { }

shared_ptr<const IExpression> MemberExpression::getContainer() const {
    return this->container;
}

shared_ptr<const IdentifierExpression> MemberExpression::getMember() const {
    return this->member;
}

MemberExpressionType MemberExpression::getType() const {
    return this->type;
}
