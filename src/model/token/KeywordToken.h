#pragma once

#include "Token.h"
#include "Keyword.h"

namespace c4 {
    namespace model {
        namespace token {
            class KeywordToken : public Token {
            public:
                const Keyword keyword;

                KeywordToken(
                    TokenPosition position,
                    Keyword keyword
                ) : Token(position), keyword(keyword) { }

                ~KeywordToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }
            };
        }
    }
}
