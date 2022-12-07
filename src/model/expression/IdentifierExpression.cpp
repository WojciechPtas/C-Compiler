#include "IdentifierExpression.h"

using namespace c4::model::expression;
using namespace std;

IdentifierExpression::IdentifierExpression(const std::string &identifier) :
    identifier(identifier) { }

const string &IdentifierExpression::getIdentifier() const {
    return this->identifier;
}
