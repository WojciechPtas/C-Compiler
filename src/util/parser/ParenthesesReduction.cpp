#include "ParenthesesReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

ParenthesesReduction::ParenthesesReduction() : PureReduction(3, 1) { }

shared_ptr<const IExpression> ParenthesesReduction::reduce(
    shared_ptr<vector<shared_ptr<const IExpression>>> consumedExpressions
) const {
    // NOTE:    We do nothing else but unpacking expressions from their
    //          parentheses. We do not need to represent parenthesis in any way
    //          in our AST.

    return consumedExpressions->back();
}
