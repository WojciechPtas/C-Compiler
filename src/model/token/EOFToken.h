#pragma once

#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class EOFToken : public Token {
            public:
                EOFToken(
                    TokenPosition position
                ) : Token(position) { }

                ~EOFToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                virtual std::shared_ptr<Token> convertDigraph() const override {
                    return std::make_shared<EOFToken>(*this);
                }
            };
        }
    }
}
