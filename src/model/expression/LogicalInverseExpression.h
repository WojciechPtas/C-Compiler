#pragma once

#include "UnaryExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class LogicalInverseExpression : public UnaryExpression {
            public:
                LogicalInverseExpression(const IExpression &expression) :
                    UnaryExpression(expression) { }

                virtual ~LogicalInverseExpression() { }
            };
        }
    }
}
