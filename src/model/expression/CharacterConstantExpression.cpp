#include "CharacterConstantExpression.h"

using namespace c4::model::expression;
using namespace std;

CharacterConstantExpression::CharacterConstantExpression(const string &value) :
    ConstantExpression(value) { }
