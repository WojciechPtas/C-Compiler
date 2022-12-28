#pragma once

#include <type_traits>

namespace c4 {
    namespace model {
        namespace expression {
            enum class MemberExpressionType : unsigned char {
                Direct  = 0x01,
                Pointer = 0x02
            };

            inline MemberExpressionType operator| (
                MemberExpressionType l,
                MemberExpressionType r
            ) {
                return MemberExpressionType(
                    std::underlying_type<MemberExpressionType>::type(l) |
                    std::underlying_type<MemberExpressionType>::type(r)
                );
            }
        }
    }
}
