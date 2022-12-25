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
                MemberExpression(
                    MemberExpressionType type,
                    const std::shared_ptr<const IExpression> container,
                    const std::shared_ptr<const IdentifierExpression> member
                );

                virtual ~MemberExpression() { }

                std::shared_ptr<const IExpression> getContainer() const;
                std::shared_ptr<const IdentifierExpression> getMember() const;
                MemberExpressionType getType() const;

            private:
                const std::shared_ptr<const IExpression> container;
                const std::shared_ptr<const IdentifierExpression> member;
                const MemberExpressionType type;
            };
        }
    }
}
