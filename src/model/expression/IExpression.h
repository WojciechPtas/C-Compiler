#pragma once

#include <memory>

#include "IExpressionVisitor.h"
#include "IExpressionCodeGenVisitor.h"
#include "../token/Token.h"
#include "../ASTNode.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IExpression : public model::IASTNode {
            public:
                IExpression(const std::shared_ptr<const model::token::Token> firstTerminal)
                : model::IASTNode(firstTerminal) {}

                virtual ~IExpression() { }
                virtual void accept(IExpressionVisitor &visitor) const = 0;
                virtual ctype::CTypedValue getLValue(IExpressionCodeGenVisitor &cg) const = 0;
                virtual ctype::CTypedValue getRValue(IExpressionCodeGenVisitor &cg) const = 0;


                virtual bool isTerminal() const {
                    return false;
                }

                virtual void accept(util::IASTVisitor& visitor) const override {
                    (void) visitor;
                    throw std::logic_error("Use IExprVisitor for visitors instead");
                }

            };
        }
    }
}
