#include "BinaryExpressionUtilities.h"

using namespace c4::model::expression;
using namespace std;

static inline void _decomposeOne(
    set<BinaryExpressionType> &dst,
    BinaryExpressionType compoundType,
    BinaryExpressionType testedType
);

unique_ptr<set<BinaryExpressionType>> c4::util::expression::decompose(
    BinaryExpressionType t
) {
    auto r = make_unique<set<BinaryExpressionType>>();

    _decomposeOne(*r, t, BinaryExpressionType::Assignment);
    _decomposeOne(*r, t, BinaryExpressionType::Equal);
    _decomposeOne(*r, t, BinaryExpressionType::LessThan);
    _decomposeOne(*r, t, BinaryExpressionType::LogicalAnd);
    _decomposeOne(*r, t, BinaryExpressionType::LogicalOr);
    _decomposeOne(*r, t, BinaryExpressionType::Multiplication);
    _decomposeOne(*r, t, BinaryExpressionType::Subtraction);
    _decomposeOne(*r, t, BinaryExpressionType::Sum);
    _decomposeOne(*r, t, BinaryExpressionType::Unequal);

    return r;
}

static inline void _decomposeOne(
    set<BinaryExpressionType> &dst,
    BinaryExpressionType compoundType,
    BinaryExpressionType testedType
) {
    if ((compoundType & testedType) == testedType) {
        dst.insert(testedType);
    }
}
