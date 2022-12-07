#pragma once

#include "IExpression.h"
#include "IdentifierExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class MemberExpression : public IExpression {
            public:
                MemberExpression(
                    const IExpression &container,
                    const IdentifierExpression &member
                );

                virtual ~MemberExpression() { }

                const IExpression &getContainer() const;
                const IdentifierExpression &getMember() const;

            private:
                const IExpression container;
                const IdentifierExpression member;
            };
        }
    }
}
