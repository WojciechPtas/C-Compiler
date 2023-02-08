#pragma once

#include <memory>

#include "IExpressionVisitor.h"
#include "IExpressionCodeGenVisitor.h"
#include "../token/Token.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IExpression {
            public:
                std::shared_ptr<const model::token::Token> firstTerminal = nullptr;

                IExpression(const std::shared_ptr<const model::token::Token> firstTerminal)
                : firstTerminal(firstTerminal) {}

                virtual ~IExpression() { }
                virtual void accept(IExpressionVisitor &visitor) const = 0;
                virtual ctype::CTypedValue getLValue(IExpressionCodeGenVisitor &cg) const = 0;
                virtual ctype::CTypedValue getRValue(IExpressionCodeGenVisitor &cg) const = 0;


                virtual bool isTerminal() const {
                    return false;
                }
            };
        }
    }
}
