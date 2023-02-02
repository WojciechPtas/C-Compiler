#pragma once
#include <string>
#include "IDeclaration.h"

namespace c4::model::declaration{
    class StructUnionSpecifier : public IDeclaration{
        public:
        std::string name;
        std::shared_ptr<IDeclaration> declarations;
       
        StructUnionSpecifier(std::string name,
        std::shared_ptr<IDeclaration> declarations,
        std::shared_ptr<token::Token> token) :  IDeclaration(token), name(name), declarations(declarations){}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}