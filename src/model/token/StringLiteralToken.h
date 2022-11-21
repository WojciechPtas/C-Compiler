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

                void accept(
                    util::token::ITokenVisitor &visitor
                ) const override;

                const std::string getValue() const;

            private:
                const std::string value;
            };
        }
    }
}
