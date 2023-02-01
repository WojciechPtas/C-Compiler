#pragma once
#include "IDeclaration.h"
// #include "ParameterTypeList.h"
namespace c4::model::declaration{
    class DirectDeclarator2 : public IDeclaration{
        public:
        std::shared_ptr<IDeclaration> list;
        std::shared_ptr<IDeclaration> declarator;
        DirectDeclarator2(
            std::shared_ptr<IDeclaration> list,
            std::shared_ptr<IDeclaration> declarator) : list(list), declarator(declarator) {}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }    
    };
}