#include "UnaryExpressionUtilities.h"

using namespace c4::model::expression;
using namespace std;

static inline void _decomposeOne(
    set<UnaryExpressionType> &dst,
    UnaryExpressionType compoundType,
    UnaryExpressionType testedType
);

unique_ptr<set<UnaryExpressionType>> c4::util::expression::decompose(
    UnaryExpressionType t
) {
    auto r = make_unique<set<UnaryExpressionType>>();

    _decomposeOne(*r, t, UnaryExpressionType::AdditiveInverse);
    _decomposeOne(*r, t, UnaryExpressionType::AddressOf);
    _decomposeOne(*r, t, UnaryExpressionType::Indirection);
    _decomposeOne(*r, t, UnaryExpressionType::LogicalInverse);
    _decomposeOne(*r, t, UnaryExpressionType::Sizeof);

    return r;
}

static inline void _decomposeOne(
    set<UnaryExpressionType> &dst,
    UnaryExpressionType compoundType,
    UnaryExpressionType testedType
) {
    if ((compoundType & testedType) == testedType) {
        dst.insert(testedType);
    }
}
