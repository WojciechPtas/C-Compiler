#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace model {
        namespace token {
            class DecimalConstantToken : public ConstantToken {
            public:
                DecimalConstantToken(TokenPosition position, std::string value);
                virtual ~DecimalConstantToken() { }

                void accept(
                    util::token::ITokenVisitor &visitor
                ) const override;

                const std::string &getValue() const;

            private:
                const std::string value;
            };
        }
    }
}
