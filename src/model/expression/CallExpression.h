#pragma once

#include <vector>
#include <memory>
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
                    shared_ptr<const IExpression>& called,
                    const std::shared_ptr<const model::token::Token> firstTerminal
                ) : IExpression(firstTerminal), called(called) { }

                CallExpression(
                    const shared_ptr<const IExpression>& called,
                    const shared_ptr<const CallArguments>& arguments,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), called(called), arguments(*arguments) { }
                
                ~CallExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                virtual ctype::CTypedValue getLValue(IExpressionCodeGenVisitor &cg) const override {
                    return cg.visitLValue(*this);
                }

                virtual ctype::CTypedValue getRValue(IExpressionCodeGenVisitor &cg) const override {
                    return cg.visitRValue(*this);
                }
            };
        }
    }
}
