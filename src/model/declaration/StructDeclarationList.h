#pragma once
#include <vector>

namespace c4::model::declaration{
    class StructDeclarationList: public IDeclaration{
        public:
        std::vector<std::shared_ptr<IDeclaration>> declarations;        
        StructDeclarationList(std::vector<std::shared_ptr<IDeclaration>> declarations) : declarations(declarations){}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };

}