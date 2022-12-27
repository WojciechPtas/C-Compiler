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
                    const std::shared_ptr<const IExpression> expression
                ) : expression(expression), type(type) { }

                ~UnaryExpression() { }
            };
        }
    }
}
