#pragma once

#include <memory>

#include "BinaryExpressionType.h"
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class BinaryExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> left;
                const std::shared_ptr<const IExpression> right;
                const BinaryExpressionType type;

                BinaryExpression(
                    BinaryExpressionType type,
                    const std::shared_ptr<const IExpression> left,
                    const std::shared_ptr<const IExpression> right,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), left(left), right(right), type(type) { }

                ~BinaryExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
