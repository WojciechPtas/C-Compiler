#pragma once
#include "IDeclaration.h"
// #include "ParameterTypeList.h"
namespace c4::model::declaration{
    class DirectDeclarator2 : public IDeclaration{
        private:
        std::shared_ptr<IDeclaration> list;
        std::shared_ptr<IDeclaration> declarator;
        public:
        DirectDeclarator2(
            std::shared_ptr<IDeclaration> list,
            std::shared_ptr<IDeclaration> declarator) : list(list), declarator(declarator) {}
            
    };
}