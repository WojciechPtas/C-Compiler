#pragma once
#include <string>
#include "Declarator.h"
#include "ParameterTypeList.h"
#include "DirectDeclarator2.h"
namespace c4::model::declaration{
    class DirectDeclarator : public IDeclaration{
        private:
            std::string identifier;
            std::shared_ptr<Declarator> declarator;
            std::shared_ptr<DirectDeclarator2> direct_declarator;
    

        public:
            DirectDeclarator(std::string identifier, //REDO
                std::shared_ptr<Declarator> declarator,
                std::shared_ptr<DirectDeclarator2> direct_declarator
             ) : identifier(identifier), 
                declarator(declarator), direct_declarator(direct_declarator){};
    };
}