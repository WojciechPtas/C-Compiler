#pragma once
#include <memory>
#include <vector>
#include "../statement/IStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class IDeclaration:public statement::IStatement{;
            public:
            virtual void accept(statement::IASTVisitor& visitor)const =0;
            };
        }
        }
    }