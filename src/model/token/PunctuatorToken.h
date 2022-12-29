#pragma once

#include "Token.h"
#include "Punctuator.h"

namespace c4 {
    namespace model {
        namespace token {
            class PunctuatorToken : public Token {
            public:
                const Punctuator punctuator;

                PunctuatorToken(
                    TokenPosition position,
                    Punctuator punctuator
                ) : Token(position), punctuator(punctuator) { }

                ~PunctuatorToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
