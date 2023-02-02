#pragma once
#include <memory>
#include <vector>
#include "../token/Token.h"
#include "../IASTVisitor.h"
namespace c4 {
    namespace model {
        namespace statement {
            class IStatement {
                public:
                IStatement(std::shared_ptr<token::Token> first_token) : first_token(first_token) {}
                virtual void accept(IASTVisitor& visitor)const =0;
                std::shared_ptr<token::Token> first_token;
            };
        }
    }
}
