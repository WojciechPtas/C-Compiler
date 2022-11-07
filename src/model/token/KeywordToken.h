#pragma once

#include "Token.h"
#include "Keyword.h"

namespace c4 {
    namespace model {
        namespace token {
            class KeywordToken : public Token {
            public:
                KeywordToken(TokenPosition position, Keyword keyword);
                virtual ~KeywordToken() { }

                void accept(util::token::ITokenVisitor &visitor) override;

                Keyword getKeyword();

            private:
                Keyword keyword;
            };
        }
    }
}
