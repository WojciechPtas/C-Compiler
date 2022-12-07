#pragma once

#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class UnaryExpression : public IExpression {
            public:
                UnaryExpression(const IExpression &expression);
                virtual ~UnaryExpression() { }

                const IExpression &getExpression() const;

            private:
                const IExpression expression;
            };
        }
    }
}
