#pragma once

#include <vector>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class CallExpression : public IExpression {
            public:
                CallExpression(
                    const std::vector<IExpression> &arguments
                );

                virtual ~CallExpression() { }

                const std::vector<IExpression> &getArguments() const;

            private:
                const std::vector<IExpression> arguments;
            };
        }
    }
}
