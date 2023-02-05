#pragma once
#include <vector>
#include "ParameterDeclaration.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterTypeList :public IDeclaration{
             public:
                std::vector<std::shared_ptr<ParameterDeclaration>> params;
                ParameterTypeList(std::vector<std::shared_ptr<ParameterDeclaration>>& params,
                std::shared_ptr<token::Token> token=nullptr) : IDeclaration(token), params(params) {};
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
        }
    }
}