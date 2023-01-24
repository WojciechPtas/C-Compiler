#pragma once
#include <string>
#include "Declarator.h"
#include "ParameterTypeList.h"
#include "IDeclaration.h"
namespace c4::model::declaration{
    class DirectDeclarator : public IDeclaration{
        private:
            std::string identifier;
            std::shared_ptr<Declarator> declarator;
            std::shared_ptr<DirectDeclarator> direct_declarator;
            std::shared_ptr<ParameterTypeList> param;

        public:
            DirectDeclarator(std::string identifier, //REDO
                std::shared_ptr<Declarator> declarator,
                std::shared_ptr<DirectDeclarator> direct_declarator,
                std::shared_ptr<ParameterTypeList> param) : identifier(identifier), 
                declarator(declarator), direct_declarator(direct_declarator), param(param) {};
    };
}