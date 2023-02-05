#pragma once
#include "IDeclaration.h"
#include "ParameterTypeList.h"
namespace c4::model::declaration{
    class DirectDeclarator2 : public IDeclaration{
        public:
        std::shared_ptr<ParameterTypeList> list;
        std::shared_ptr<DirectDeclarator2> declarator;
        DirectDeclarator2(
            std::shared_ptr<ParameterTypeList> list,
            std::shared_ptr<DirectDeclarator2> declarator,
            std::shared_ptr<token::Token> token=nullptr) : IDeclaration(token), list(list), 
            declarator(declarator){}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }    
    };
}