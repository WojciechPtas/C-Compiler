#pragma once

#include <memory>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndexExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IExpression> index;

                IndexExpression(
                    std::shared_ptr<const IExpression> container,
                    std::shared_ptr<const IExpression> index,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), container(container), index(index) { }

                ~IndexExpression() { }

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
