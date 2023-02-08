#pragma once

#include <memory>

#include "IExpression.h"
#include "IdentifierExpression.h"
#include "MemberExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class MemberExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IdentifierExpression> member;
                const MemberExpressionType type;

                MemberExpression(
                    MemberExpressionType type,
                    const std::shared_ptr<const IExpression> container,
                    const std::shared_ptr<const IdentifierExpression> member,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), container(container), member(member), type(type) { }

                ~MemberExpression() { }

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
