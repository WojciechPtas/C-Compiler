#pragma once

#include <memory>

#include "IExpression.h"
#include "../declaration/TypeName.h"
namespace c4 {
    namespace model {
        namespace expression {
            class SizeOfType : public IExpression {
            public:
                std::shared_ptr<const model::declaration::TypeName> type;

                SizeOfType(
                    const std::shared_ptr<const model::declaration::TypeName> &type,
                    const std::shared_ptr<const model::token::Token> &firstTerminal
                ) : IExpression(firstTerminal), type(type) {}

                ~SizeOfType() { }

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
