#pragma once
#include <memory>
#include <vector>
#include "../ASTNode.h"
namespace c4 {
    namespace model {
        namespace statement {
            class IStatement : public IASTNode{
                public:
                IStatement(std::shared_ptr<const token::Token> first_token) : IASTNode(first_token) {}
                virtual void accept(util::IASTVisitor& visitor)const =0;
            };
        }
    }
}
