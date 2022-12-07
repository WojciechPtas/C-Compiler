#pragma once

#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndexExpression : public IExpression {
            public:
                IndexExpression(
                    const IExpression &container,
                    const IExpression &index
                );

                virtual ~IndexExpression() { }

                const IExpression &getContainer() const;
                const IExpression &getIndex() const;

            private:
                const IExpression container;
                const IExpression index;
            };
        }
    }
}
