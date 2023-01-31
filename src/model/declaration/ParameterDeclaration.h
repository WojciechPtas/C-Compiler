#pragma once
#include "../token/Keyword.h"
#include "IDeclaration.h"

namespace c4 {
    namespace model {
        namespace declaration {
            class ParameterDeclaration :public IDeclaration{
            private:
                std::shared_ptr<IDeclaration> type;
                std::shared_ptr<IDeclaration> dec;
            public:
                ParameterDeclaration(std::shared_ptr<IDeclaration> type,
                std::shared_ptr<IDeclaration> dec) : type(type), dec(dec) {}
            };
        }
    }
}