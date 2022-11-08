#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class StringLiteralToken : public Token {
            public:
                StringLiteralToken(TokenPosition position, std::string value);
                virtual ~StringLiteralToken() { }

                void accept(util::token::ITokenVisitor &visitor) override;

                std::string getValue();

            private:
                std::string value;
            };
        }
    }
}
