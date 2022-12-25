#include "../../model/expression/IndexExpression.h"
#include "IndexReduction.h"

using namespace c4::model::expression;
using namespace c4::util::parser;
using namespace std;

IndexReduction::IndexReduction() : PureReduction(4, 2) { }

shared_ptr<const IExpression> IndexReduction::reduce(
    const vector<shared_ptr<const IExpression>> &consumedExpressions
) const {
    auto container = consumedExpressions[0];
    auto index = consumedExpressions[1];

    return make_shared<IndexExpression>(container, index);
}
