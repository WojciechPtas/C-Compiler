#pragma once

#include <type_traits>

#include "../../expression/BinaryExpressionType.h"
#include "../../expression/MemberExpressionType.h"
#include "../../expression/UnaryExpressionType.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                enum class ExpressionType : unsigned short {
                    Binary        = 0x01,
                    Call          = 0x02,
                    Conditional   = 0x04,
                    Constant      = 0x08,
                    Identifier    = 0x10,
                    Index         = 0x20,
                    Member        = 0x40,
                    Unary         = 0x80,
                    CallArguments = 0x100,
                    TypeInSizeof = 0x200
                };

                struct ExpressionCondition {
                    ExpressionType typeMask;
                    expression::BinaryExpressionType binaryExpressionMask;
                    expression::MemberExpressionType memberExpressionMask;
                    expression::UnaryExpressionType unaryExpressionMask;
                };

                inline ExpressionType operator& (
                    ExpressionType l,
                    ExpressionType r
                ) {
                    return ExpressionType(
                        std::underlying_type<ExpressionType>::type(l) &
                        std::underlying_type<ExpressionType>::type(r)
                    );
                }

                inline ExpressionType operator| (
                    ExpressionType l,
                    ExpressionType r
                ) {
                    return ExpressionType(
                        std::underlying_type<ExpressionType>::type(l) |
                        std::underlying_type<ExpressionType>::type(r)
                    );
                }

                inline ExpressionCondition operator| (
                    ExpressionCondition l,
                    ExpressionCondition r
                ) {
                    return {
                        l.typeMask | r.typeMask,
                        l.binaryExpressionMask | r.binaryExpressionMask,
                        l.memberExpressionMask | r.memberExpressionMask,
                        l.unaryExpressionMask | r.unaryExpressionMask
                    };
                }
            }
        }
    }
}
