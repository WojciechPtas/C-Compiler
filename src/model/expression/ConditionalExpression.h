#pragma once

#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class ConditionalExpression : public IExpression {
            public:
                ConditionalExpression(
                    const IExpression &condition,
                    const IExpression &thenCase,
                    const IExpression &elseCase
                );
                virtual ~ConditionalExpression() { }

                const IExpression &getCondition() const;
                const IExpression &getElseCase() const;
                const IExpression &getThenCase() const;

            private:
                const IExpression condition;
                const IExpression elseCase;
                const IExpression thenCase;
            };
        }
    }
}
