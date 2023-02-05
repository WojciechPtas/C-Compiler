#pragma once
#include <memory>
#include <vector>
#include "../statement/IStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class IDeclaration: public statement::IStatement {
            public:
            IDeclaration(std::shared_ptr<const token::Token> token) : statement::IStatement(token) {}
            virtual void accept(util::IASTVisitor& visitor)const =0;
            };
        }
        }
    }