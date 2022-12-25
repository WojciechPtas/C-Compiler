#pragma once

#include <memory>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class ConditionalExpression : public IExpression {
            public:
                ConditionalExpression(
                    const std::shared_ptr<const IExpression> condition,
                    const std::shared_ptr<const IExpression> thenCase,
                    const std::shared_ptr<const IExpression> elseCase
                );

                virtual ~ConditionalExpression() { }

                std::shared_ptr<const IExpression> getCondition() const;
                std::shared_ptr<const IExpression> getElseCase() const;
                std::shared_ptr<const IExpression> getThenCase() const;

            private:
                const std::shared_ptr<const IExpression> condition;
                const std::shared_ptr<const IExpression> elseCase;
                const std::shared_ptr<const IExpression> thenCase;
            };
        }
    }
}
