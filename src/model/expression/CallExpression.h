#pragma once

#include <vector>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class CallExpression : public IExpression {
            public:
                const std::vector<IExpression> arguments;

                CallExpression(
                    const std::vector<IExpression> &arguments
                ) : arguments(arguments) { }
                
                ~CallExpression() { }
            };
        }
    }
}
