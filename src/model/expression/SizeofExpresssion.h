#pragma once

#include "UnaryExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class SizeofExpression : public UnaryExpression {
            public:
                SizeofExpression(const IExpression &expression) :
                    UnaryExpression(expression) { }

                virtual ~SizeofExpression() { }
            };
        }
    }
}
