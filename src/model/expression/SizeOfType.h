#pragma once

#include <memory>

#include "IExpression.h"
#include "../token/Keyword.h"

namespace c4 {
    namespace model {
        namespace expression {
            class SizeOfType : public IExpression {
            public:
                const token::Keyword type;

                SizeOfType(
                    const token::Keyword type
                ) : type(type) {
                    //must make sure it is a type!
                }

                ~SizeOfType() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
