#pragma once

namespace c4 {
    namespace model {
        namespace expression {
            enum class UnaryExpressionType {
                AdditiveInverse,
                AddressOf,
                Indirection,
                LogicalInverse,
                Sizeof
            };
        }
    }
}
