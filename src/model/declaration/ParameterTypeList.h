#pragma once
#include <vector>
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterTypeList :public IDeclaration{
            private:
               
               std::vector<std::shared_ptr<IDeclaration>> params;

            public:
                ParameterTypeList(std::vector<std::shared_ptr<IDeclaration>>& params) : params(params) {};
                std::vector<std::shared_ptr<IDeclaration>> getParams(){return params;}
                };
        }
    }
}