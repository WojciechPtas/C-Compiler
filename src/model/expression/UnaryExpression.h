#pragma once

#include <memory>

#include "IExpression.h"
#include "UnaryExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class UnaryExpression : public IExpression {
            public:
                UnaryExpression(
                    UnaryExpressionType type,
                    const std::shared_ptr<const IExpression> expression
                );

                virtual ~UnaryExpression() { }

                std::shared_ptr<const IExpression> getExpression() const;
                UnaryExpressionType getType() const;

            private:
                const std::shared_ptr<const IExpression> expression;
                const UnaryExpressionType type;
            };
        }
    }
}
