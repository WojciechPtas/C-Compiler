#pragma once

#include <string>

#include "../ConstantType.h"
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class ConstantExpression : public IExpression {
            public:
                const ConstantType type;
                const std::string value;

                ConstantExpression(ConstantType type, const std::string &value) :
                    type(type),
                    value(value) { }

                ~ConstantExpression() { }
            };
        }
    }
}
