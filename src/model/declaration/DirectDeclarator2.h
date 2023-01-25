#pragma once
#include "IDeclaration.h"
#include "ParameterTypeList.h"
namespace c4::model::declaration{
    class DirectDeclarator2 : public IDeclaration{
        private:
        std::shared_ptr<ParameterTypeList> list;
        std::shared_ptr<DirectDeclarator2> declarator;
        public:
        DirectDeclarator2(
            std::shared_ptr<ParameterTypeList> list,
            std::shared_ptr<DirectDeclarator2> declarator) : list(list), declarator(declarator) {}
            
    };
}