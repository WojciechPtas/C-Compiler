#pragma once

#include <memory>

#include "IExpression.h"
#include "UnaryExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class UnaryExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> expression;
                const UnaryExpressionType type;

                UnaryExpression(
                    UnaryExpressionType type,
                    const std::shared_ptr<const IExpression> expression,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), expression(expression), type(type) { }

                ~UnaryExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
