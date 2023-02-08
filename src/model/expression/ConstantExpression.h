#pragma once

#include <string>

#include "../ConstantType.h"
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class ConstantExpression : public IExpression {
            public:
                const ConstantType type;
                const std::string value;

                ConstantExpression(
                    ConstantType type, 
                    const std::string &value,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), type(type),
                    value(value) { }

                ~ConstantExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                bool isTerminal() const override {
                    return true;
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
