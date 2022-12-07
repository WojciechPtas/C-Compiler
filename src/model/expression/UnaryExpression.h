#pragma once

#include "IExpression.h"
#include "UnaryExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class UnaryExpression : public IExpression {
            public:
                UnaryExpression(
                    UnaryExpressionType type,
                    const IExpression &expression
                );

                virtual ~UnaryExpression() { }

                const IExpression &getExpression() const;
                const UnaryExpressionType getType() const;

            private:
                const IExpression expression;
                const UnaryExpressionType type;
            };
        }
    }
}
