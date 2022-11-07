#pragma once

#include "Token.h"
#include "Keyword.h"

namespace c4 {
    namespace token {
        class KeywordToken : public Token {
        public:
            KeywordToken(TokenPosition position, Keyword keyword);
            virtual ~KeywordToken() { }

            void accept(ITokenVisitor &visitor) override;

            Keyword getKeyword();

        private:
            Keyword keyword;
        };
    }
}
