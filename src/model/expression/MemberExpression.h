#pragma once

#include <memory>

#include "IExpression.h"
#include "IdentifierExpression.h"
#include "MemberExpressionType.h"

namespace c4 {
    namespace model {
        namespace expression {
            class MemberExpression : public IExpression {
            public:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IdentifierExpression> member;
                const MemberExpressionType type;

                MemberExpression(
                    MemberExpressionType type,
                    const std::shared_ptr<const IExpression> container,
                    const std::shared_ptr<const IdentifierExpression> member
                ) : container(container), member(member), type(type) { }

                ~MemberExpression() { }
            };
        }
    }
}
