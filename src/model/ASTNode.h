#pragma once
#include <memory>

#include "../util/ASTVisitors/IASTVisitor.h"
#include "token/Token.h"
namespace c4::model
{
    class IASTNode{
        public:
        std::shared_ptr<const token::Token> firstTerminal;
        IASTNode(std::shared_ptr<const token::Token> token) : firstTerminal(token){}
        virtual void accept(util::IASTVisitor& visitor)const = 0;
    };
} // namespace c4::model
