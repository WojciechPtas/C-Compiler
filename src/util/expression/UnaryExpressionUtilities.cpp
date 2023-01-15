#include <stdexcept>
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

const string c4::util::expression::stringify(UnaryExpressionType type) {
    switch (type) {
        case UnaryExpressionType::AdditiveInverse:  return "-";
        case UnaryExpressionType::AddressOf:        return "&";
        case UnaryExpressionType::Indirection:      return "*";
        case UnaryExpressionType::LogicalInverse:   return "!";
        case UnaryExpressionType::Sizeof:           return "sizeof";
        default:
            throw logic_error("Unimplemented unary expression type");
    }
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
