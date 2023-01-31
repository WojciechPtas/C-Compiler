#pragma once
#include "IDeclaration.h"
namespace c4::model::declaration{
    class Root: public IDeclaration{
        private:
        std::vector<std::shared_ptr<IDeclaration>> definitions;
        public:
        Root(std::vector<std::shared_ptr<IDeclaration>> definitions) :definitions(definitions) {}
    };
}