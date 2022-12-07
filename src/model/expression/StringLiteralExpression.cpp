#include "StringLiteralExpression.h"

using namespace c4::model::expression;
using namespace std;

StringLiteralExpression::StringLiteralExpression(const string &value) :
    ConstantExpression(value) { }
