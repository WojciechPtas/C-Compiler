#pragma once

#include <memory>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndexExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IExpression> index;

                IndexExpression(
                    std::shared_ptr<const IExpression> container,
                    std::shared_ptr<const IExpression> index
                ) : container(container), index(index) { }

                ~IndexExpression() { }

                void accept(IExpressionVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
