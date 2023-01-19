#pragma once
#include "../statement/IStatement.h"
#include "IExpressionVisitor.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IExpression : public IStatement{
            public:
                virtual ~IExpression() { }
                virtual void accept(IExpressionVisitor &visitor) const = 0;
            };
        }
    }
}
