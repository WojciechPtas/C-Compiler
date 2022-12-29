#include "MemberExpressionUtilities.h"

using namespace c4::model::expression;
using namespace std;

static inline void _decomposeOne(
    set<MemberExpressionType> &dst,
    MemberExpressionType compoundType,
    MemberExpressionType testedType
);

unique_ptr<set<MemberExpressionType>> c4::util::expression::decompose(
    MemberExpressionType t
) {
    auto r = make_unique<set<MemberExpressionType>>();

    _decomposeOne(*r, t, MemberExpressionType::Direct);
    _decomposeOne(*r, t, MemberExpressionType::Pointer);

    return r;
}

static inline void _decomposeOne(
    set<MemberExpressionType> &dst,
    MemberExpressionType compoundType,
    MemberExpressionType testedType
) {
    if ((compoundType & testedType) == testedType) {
        dst.insert(testedType);
    }
}
