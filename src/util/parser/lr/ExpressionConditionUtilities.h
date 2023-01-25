#pragma once

#include "../../../model/parser/lr/ExpressionCondition.h"

namespace c4 {
    namespace util {
        namespace parser {
            namespace lr {
                //Note: CallArguments is not an expression
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

                const model::parser::lr::ExpressionCondition CALL_ARGUMENTS = {
                    model::parser::lr::ExpressionType::CallArguments,
                    model::expression::BinaryExpressionType(0),
                    model::expression::MemberExpressionType(0),
                    model::expression::UnaryExpressionType(0)
                };

                const model::parser::lr::ExpressionCondition TYPE_IN_SIZEOF = {
                    model::parser::lr::ExpressionType::TypeInSizeof,
                    model::expression::BinaryExpressionType(0),
                    model::expression::MemberExpressionType(0),
                    model::expression::UnaryExpressionType(0)
                };
            }
        }
    }
}
