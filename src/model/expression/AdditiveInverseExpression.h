#pragma once

#include "UnaryExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class AdditiveInverseExpression : public UnaryExpression {
            public:
                AdditiveInverseExpression(const IExpression &expression) :
                    UnaryExpression(expression) { }

                virtual ~AdditiveInverseExpression() { }
            };
        }
    }
}
