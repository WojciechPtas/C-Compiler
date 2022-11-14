#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class ErrorToken : public Token {
            public:
                ErrorToken(
                    TokenPosition position,
                    std::string message
                );

                virtual ~ErrorToken() { }

                void accept(util::token::ITokenVisitor &visitor) override;

                const std::string &getMessage();

            private:
                std::string message;
            };
        }
    }
}
