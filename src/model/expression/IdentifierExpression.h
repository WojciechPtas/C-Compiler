#pragma once

#include <string>
#include "IExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class IdentifierExpression : public IExpression {
            public:
                IdentifierExpression(const std::string &identifier);
                virtual ~IdentifierExpression() { }

                const std::string &getIdentifier() const;

            private:
                const std::string identifier;
            };
        }
    }
}
