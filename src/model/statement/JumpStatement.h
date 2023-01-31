#pragma once
#include "IStatement.h"
#include "../expression/IExpression.h"
namespace c4 {
    namespace model {
        namespace statement {
            class JumpStatement :public IStatement {
                private:
                    std::shared_ptr<const c4::model::expression::IExpression> returnExpression;
                    std::string gotoIdentifier;
                public:
                    JumpStatement(
                        std::shared_ptr<const c4::model::expression::IExpression> returnExpression,
                        std::string gotoIdentifier
                    ) : returnExpression(returnExpression), gotoIdentifier(gotoIdentifier){};
                    std::shared_ptr<const c4::model::expression::IExpression> getIfExpression(){return returnExpression;}
                    std::string getGotoIdentifier(){return gotoIdentifier;}
                };
            }
        }
    }