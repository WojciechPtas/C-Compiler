#pragma once

#include <type_traits>

namespace c4 {
    namespace model {
        namespace expression {
            enum class UnaryExpressionType : unsigned char {
                AdditiveInverse = 0x01,
                AddressOf       = 0x02,
                Indirection     = 0x04,
                LogicalInverse  = 0x08,
                Sizeof          = 0x10
            };

            inline UnaryExpressionType operator& (
                UnaryExpressionType l,
                UnaryExpressionType r
            ) {
                return UnaryExpressionType(
                    std::underlying_type<UnaryExpressionType>::type(l) &
                    std::underlying_type<UnaryExpressionType>::type(r)
                );
            }

            inline UnaryExpressionType operator| (
                UnaryExpressionType l,
                UnaryExpressionType r
            ) {
                return UnaryExpressionType(
                    std::underlying_type<UnaryExpressionType>::type(l) |
                    std::underlying_type<UnaryExpressionType>::type(r)
                );
            }
        }
    }
}
