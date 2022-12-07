#pragma once

namespace c4 {
    namespace model {
        namespace expression {
            enum class BinaryExpressionType {
                Assignment,
                Equal,
                LogicalAnd,
                LogicalOr,
                LessThan,
                Multiplication,
                Subtraction,
                Sum,
                Unequal
            };
        }
    }
}
