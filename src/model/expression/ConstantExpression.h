#pragma once

#include <string>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            template <class TValue>
            class ConstantExpression : public IExpression {
            public:
                ConstantExpression(const TValue &value);
                virtual ~ConstantExpression() { }

                const TValue &getValue() const;

            private:
                const TValue value;
            };

            using CharacterConstantExpression =
                ConstantExpression<std::string>;

            using DecimalConstantExpression = ConstantExpression<std::string>;
            using StringLiteralExpression = ConstantExpression<std::string>;
        }
    }
}
