#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class JumpStatement : IStatement {
                private:
                    std::shared_ptr<c4::model::expression::IExpression> returnExpression;
                    std::shared_ptr<c4::model::expression::IdentifierExpression> gotoIdentifier;
                public:
                    JumpStatement(
                        std::shared_ptr<c4::model::expression::IExpression> returnExpression,
                        std::shared_ptr<c4::model::expression::IdentifierExpression> gotoIdentifier
                    ) : returnExpression(returnExpression), gotoIdentifier(gotoIdentifier){};
                    std::shared_ptr<c4::model::expression::IExpression> getIfExpression(){return returnExpression;}
                    std::shared_ptr<c4::model::expression::IdentifierExpression> getGotoIdentifier(){return gotoIdentifier;}
                };
            }
        }
    }