#pragma once

#include "UnaryExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class AddressOfExpression : public UnaryExpression {
            public:
                AddressOfExpression(const IExpression &expression) :
                    UnaryExpression(expression) { }

                virtual ~AddressOfExpression() { }
            };
        }
    }
}
