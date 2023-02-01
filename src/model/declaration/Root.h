#pragma once
#include "IDeclaration.h"
namespace c4::model::declaration{
    class Root: public IDeclaration{
        public:
        std::vector<std::shared_ptr<IDeclaration>> definitions;
        
        Root(std::vector<std::shared_ptr<IDeclaration>> definitions) :definitions(definitions) {}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };
}