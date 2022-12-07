#pragma once

#include "BinaryExpressionType.h"
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class BinaryExpression : public IExpression {
            public:
                BinaryExpression(
                    BinaryExpressionType type,
                    const IExpression &left,
                    const IExpression &right
                );

                virtual ~BinaryExpression() { }

                const IExpression &getLeft() const;
                const IExpression &getRight() const;
                BinaryExpressionType getType() const;

            private:
                const IExpression left;
                const IExpression right;
                const BinaryExpressionType type;
            };
        }
    }
}
