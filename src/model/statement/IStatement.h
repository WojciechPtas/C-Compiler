#pragma once
#include <memory>
#include <vector>
#include "../IASTVisitor.h"
namespace c4 {
    namespace model {
        namespace statement {
            class IStatement {
                public:
                virtual void accept(IASTVisitor& visitor)const =0;
            };
        }
    }
}
