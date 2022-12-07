#pragma once

#include "MemberExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class DirectMemberExpression : public MemberExpression {
            public:
                DirectMemberExpression(
                    const IExpression &container,
                    const IdentifierExpression &member
                ) : MemberExpression(container, member) { }

                virtual ~DirectMemberExpression() { }
            };
        }
    }
}
