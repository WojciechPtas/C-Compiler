#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class IdentifierToken : public Token {
            public:
                IdentifierToken(
                    TokenPosition position,
                    std::string identifier
                );

                virtual ~IdentifierToken() { }

                void accept(
                    util::token::ITokenVisitor &visitor
                ) const override;

                const std::string &getIdentifier() const;

            private:
                const std::string identifier;
            };
        }
    }
}
