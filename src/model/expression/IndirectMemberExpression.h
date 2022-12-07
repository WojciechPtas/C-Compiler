#pragma once

#include "MemberExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IndirectMemberExpression : public MemberExpression {
            public:
                IndirectMemberExpression(
                    const IExpression &container,
                    const IdentifierExpression &member
                ) : MemberExpression(container, member) { }

                virtual ~IndirectMemberExpression() { }
            };
        }
    }
}
