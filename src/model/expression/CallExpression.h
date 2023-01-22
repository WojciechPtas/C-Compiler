#pragma once

#include <vector>
#include "IExpression.h"
#include "CallArguments.h"
#include <memory>

namespace c4 {
    namespace model {
        namespace expression {
            class CallExpression : public IExpression {
            public:
                shared_ptr<const IExpression> called;
                CallArguments arguments;

                CallExpression(
                    shared_ptr<const IExpression>& called
                ) : called(called) { }

                CallExpression(
                    const shared_ptr<const IExpression>& called,
                    const shared_ptr<const CallArguments>& arguments
                ) : called(called), arguments(*arguments) { }
                
                ~CallExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
