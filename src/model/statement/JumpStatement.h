#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            enum class kind {_continue, _goto, _break, _return};
            class JumpStatement :public IStatement {
                public:
                    std::shared_ptr<const c4::model::expression::IExpression> returnExpression;
                    std::string gotoIdentifier;
                    kind k;
                    JumpStatement(
                        std::shared_ptr<const c4::model::expression::IExpression> returnExpression,
                        std::string gotoIdentifier,
                        kind k
                    ) : returnExpression(returnExpression),  gotoIdentifier(gotoIdentifier), k(k){};
                    void accept(IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
            }
        }
    }