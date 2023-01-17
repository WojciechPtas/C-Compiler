#pragma once

#include <vector>
#include <memory>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class CallExpression : public IExpression {
            public:
                const std::vector<std::shared_ptr<IExpression>> arguments;

                CallExpression(
                    const std::vector<std::shared_ptr<IExpression>> &arguments
                ) : arguments(arguments) { }
                
                ~CallExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
