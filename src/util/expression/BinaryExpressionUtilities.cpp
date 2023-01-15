#include <stdexcept>
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

const string c4::util::expression::stringify(BinaryExpressionType type) {
    switch (type) {
        case BinaryExpressionType::Assignment:      return "=";
        case BinaryExpressionType::Equal:           return "==";
        case BinaryExpressionType::LessThan:        return "<";
        case BinaryExpressionType::LogicalAnd:      return "&&";
        case BinaryExpressionType::LogicalOr:       return "||";
        case BinaryExpressionType::Multiplication:  return "*";
        case BinaryExpressionType::Subtraction:     return "-";
        case BinaryExpressionType::Sum:             return "+";
        case BinaryExpressionType::Unequal:         return "!=";
        default:
            throw logic_error("Unimplemented binary expression type");
    }
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
