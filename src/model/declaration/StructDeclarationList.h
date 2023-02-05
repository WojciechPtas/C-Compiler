#pragma once
#include <vector>

namespace c4::model::declaration{
    class Declaration;
    class StructDeclarationList: public IDeclaration{
        public:
        std::vector<std::shared_ptr<Declaration>> declarations;        
        StructDeclarationList(std::vector<std::shared_ptr<Declaration>> declarations,
        std::shared_ptr<token::Token> token=nullptr) :  IDeclaration(token), declarations(declarations){}
        void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
    };

}