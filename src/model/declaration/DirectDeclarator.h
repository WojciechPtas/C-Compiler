#pragma once
#include <string>
// #include "Declarator.h"
// #include "ParameterTypeList.h"
#include "DirectDeclarator2.h"
namespace c4::model::declaration{
    class Declarator;
    class DirectDeclarator : public IDeclaration{
        public:
            std::string identifier;
            std::shared_ptr<Declarator> declarator;
            std::shared_ptr<DirectDeclarator2> direct_declarator; 
        
            DirectDeclarator(std::string identifier, //REDO
                std::shared_ptr<Declarator> declarator,
                std::shared_ptr<DirectDeclarator2> direct_declarator,
                std::shared_ptr<token::Token> token=nullptr
             ) :  IDeclaration(token),identifier(identifier), 
                declarator(declarator), direct_declarator(direct_declarator){};
            void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}