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

                void accept(util::token::ITokenVisitor &visitor) override;

                std::string &getValue();

            private:
                std::string value;
            };
        }
    }
}
