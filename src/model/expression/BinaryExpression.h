#pragma once

#include <memory>

#include "BinaryExpressionType.h"
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class BinaryExpression : public IExpression {
            public:
                BinaryExpression(
                    BinaryExpressionType type,
                    const std::shared_ptr<const IExpression> left,
                    const std::shared_ptr<const IExpression> right
                );

                virtual ~BinaryExpression() { }

                std::shared_ptr<const IExpression> getLeft() const;
                std::shared_ptr<const IExpression> getRight() const;
                BinaryExpressionType getType() const;

            private:
                const std::shared_ptr<const IExpression> left;
                const std::shared_ptr<const IExpression> right;
                const BinaryExpressionType type;
            };
        }
    }
}
