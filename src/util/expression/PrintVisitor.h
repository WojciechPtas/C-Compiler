#pragma once

#include <ostream>
#include "../../model/expression/IExpressionVisitor.h"

namespace c4 {
    namespace util {
        namespace expression {
            class PrintVisitor : public model::expression::IExpressionVisitor {
            public:
                PrintVisitor(std::ostream &outputStream);
                ~PrintVisitor() { }

                void visit(
                    const model::expression::BinaryExpression &expr
                ) override;

                void visit(
                    const model::expression::CallArguments &expr
                ) override;

                void visit(
                    const model::expression::CallExpression &expr
                ) override;

                void visit(
                    const model::expression::ConditionalExpression &expr
                ) override;

                void visit(
                    const model::expression::ConstantExpression &expr
                ) override;

                void visit(
                    const model::expression::IdentifierExpression &expr
                ) override;

                void visit(
                    const model::expression::IndexExpression &expr
                ) override;

                void visit(
                    const model::expression::MemberExpression &expr
                ) override;

                void visit(
                    const model::expression::UnaryExpression &expr
                ) override;

            private:
                std::ostream &outputStream;
            };
        }
    }
}
