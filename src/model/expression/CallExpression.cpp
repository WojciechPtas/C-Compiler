#include "CallExpression.h"

using namespace c4::model::expression;
using namespace std;

CallExpression::CallExpression(const vector<IExpression> &arguments) :
    arguments(arguments) { }

const vector<IExpression> &CallExpression::getArguments() const {
    return this->arguments;
}
