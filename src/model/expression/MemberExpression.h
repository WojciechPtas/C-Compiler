#pragma once

#include "IExpression.h"
#include "IdentifierExpression.h"
#include "MemberExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class MemberExpression : public IExpression {
            public:
                MemberExpression(
                    MemberExpressionType type,
                    const IExpression &container,
                    const IdentifierExpression &member
                );

                virtual ~MemberExpression() { }

                const IExpression &getContainer() const;
                const IdentifierExpression &getMember() const;
                MemberExpressionType getType() const;

            private:
                const IExpression container;
                const IdentifierExpression member;
                const MemberExpressionType type;
            };
        }
    }
}
