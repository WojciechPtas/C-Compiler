#pragma once

#include "Token.h"
#include "Punctuator.h"

namespace c4 {
    namespace model {
        namespace token {
            class PunctuatorToken : public Token {
            public:
                PunctuatorToken(TokenPosition position, Punctuator punctuator);
                virtual ~PunctuatorToken() { }

                void accept(
                    util::token::ITokenVisitor &visitor
                ) const override;

                Punctuator getPunctuator() const;

            private:
                Punctuator punctuator;
            };
        }
    }
}
