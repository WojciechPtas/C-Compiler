#pragma once

#include "DereferencingType.h"
#include "IExpression.h"
#include "IdentifierExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class MemberExpression : public IExpression {
            public:
                MemberExpression(
                    DereferencingType accessType,
                    const IExpression &container,
                    const IdentifierExpression &member
                );

                virtual ~MemberExpression() { }

                DereferencingType getAccessType() const;
                const IExpression &getContainer() const;
                const IdentifierExpression &getMember() const;

            private:
                const DereferencingType accessType;
                const IExpression container;
                const IdentifierExpression member;
            };
        }
    }
}
