#pragma once

#include <type_traits>

namespace c4 {
    namespace model {
        namespace expression {
            enum class BinaryExpressionType : unsigned short {
                Assignment      = 0x0001,
                Equal           = 0x0002,
                LogicalAnd      = 0x0004,
                LogicalOr       = 0x0008,
                LessThan        = 0x0010,
                Multiplication  = 0x0020,
                Subtraction     = 0x0040,
                Sum             = 0x0080,
                Unequal         = 0x0100
            };

            inline BinaryExpressionType operator| (
                BinaryExpressionType l,
                BinaryExpressionType r
            ) {
                return BinaryExpressionType(
                    std::underlying_type<BinaryExpressionType>::type(l) |
                    std::underlying_type<BinaryExpressionType>::type(r)
                );
            }
        }
    }
}
