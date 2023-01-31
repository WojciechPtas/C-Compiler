#pragma once
#include <vector>
#include "ParameterDeclaration.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterTypeList : IDeclaration{
            private:
               
               std::vector<std::shared_ptr<ParameterDeclaration>> params;

            public:
                ParameterTypeList(std::vector<std::shared_ptr<ParameterDeclaration>>& params) : params(params) {};
                std::vector<std::shared_ptr<ParameterDeclaration>> getParams(){return params;}
                };
        }
    }
}