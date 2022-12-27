#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class IdentifierToken : public Token {
            public:
                const std::string identifier;

                IdentifierToken(
                    TokenPosition position,
                    std::string identifier
                ) : Token(position), identifier(identifier) { }

                ~IdentifierToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
