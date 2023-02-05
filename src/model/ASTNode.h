#pragma once
#include <memory>

#include "IASTVisitor.h"
#include "token/Token.h"
namespace c4::model
{
    class IASTNode{
        public:
        std::shared_ptr<const token::Token> token;
        IASTNode(std::shared_ptr<const token::Token> token) : token(token){}
        virtual void accept(IASTVisitor& visitor)const = 0;
    };
} // namespace c4::model
