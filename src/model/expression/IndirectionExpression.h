#pragma once

#include "UnaryExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndirectionExpression : public UnaryExpression {
            public:
                IndirectionExpression(const IExpression &expression) :
                    UnaryExpression(expression) { }

                virtual ~IndirectionExpression() { }
            };
        }
    }
}
