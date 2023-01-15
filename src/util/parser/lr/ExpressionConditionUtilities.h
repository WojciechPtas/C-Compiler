#pragma once

#include "../../../model/parser/lr/ExpressionCondition.h"

namespace c4 {
    namespace util {
        namespace parser {
            namespace lr {
                const model::parser::lr::ExpressionCondition ANY_EXPRESSION = {
                    model::parser::lr::ExpressionType(0xFF),
                    model::expression::BinaryExpressionType(0x1FF),
                    model::expression::MemberExpressionType(0x3),
                    model::expression::UnaryExpressionType(0x1F)
                };

                inline model::parser::lr::ExpressionCondition BINARY_EXPRESSION(
                    model::expression::BinaryExpressionType type
                ) {
                    model::parser::lr::ExpressionCondition condition;
                    condition.typeMask =
                        model::parser::lr::ExpressionType::Binary;

                    condition.binaryExpressionMask = type;

                    return condition;
                }
            }
        }
    }
}
