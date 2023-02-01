#pragma once
#include <vector>
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterTypeList :public IDeclaration{
             public:
               
               std::vector<std::shared_ptr<IDeclaration>> params;

           
                ParameterTypeList(std::vector<std::shared_ptr<IDeclaration>>& params) : params(params) {};
                std::vector<std::shared_ptr<IDeclaration>> getParams(){return params;}
                void accept(statement::IASTVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
        }
    }
}