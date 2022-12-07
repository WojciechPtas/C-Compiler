#include "ConstantExpression.h"

using namespace c4::model::expression;
using namespace std;

template <class TValue>
ConstantExpression<TValue>::ConstantExpression(const TValue &value) :
    value(value) { }

template <class TValue>
const TValue &ConstantExpression<TValue>::getValue() const {
    return this->value;
}

// NOTE:    The following is necessary to make the C++ compiler including the
//          implementation for the ConstantExpression<string> class in the
//          generated object file that corresponds to this source file.

template class ConstantExpression<string>;
