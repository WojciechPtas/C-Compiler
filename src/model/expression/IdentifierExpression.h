#pragma once

#include <string>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IdentifierExpression : public IExpression {
            public:
                const std::string identifier;

                IdentifierExpression(
                    const std::string &identifier,
                    const std::shared_ptr<const model::token::Token>& firstTerminal
                ) : IExpression(firstTerminal), identifier(identifier) { }

                ~IdentifierExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                bool isTerminal() const override {
                    return true;
                }
            };
        }
    }
}
