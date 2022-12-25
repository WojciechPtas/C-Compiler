#pragma once

#include <memory>

#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndexExpression : public IExpression {
            public:
                IndexExpression(
                    std::shared_ptr<const IExpression> container,
                    std::shared_ptr<const IExpression> index
                );

                virtual ~IndexExpression() { }

                std::shared_ptr<const IExpression> getContainer() const;
                std::shared_ptr<const IExpression> getIndex() const;

            private:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IExpression> index;
            };
        }
    }
}
