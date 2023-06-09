#pragma once

#include <memory>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class ConditionalExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> condition;
                const std::shared_ptr<const IExpression> elseCase;
                const std::shared_ptr<const IExpression> thenCase;

                ConditionalExpression(
                    const std::shared_ptr<const IExpression> condition,
                    const std::shared_ptr<const IExpression> thenCase,
                    const std::shared_ptr<const IExpression> elseCase,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), condition(condition),
                    elseCase(elseCase),
                    thenCase(thenCase) { }

                virtual ~ConditionalExpression() { }

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
