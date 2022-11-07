#pragma once

#include "IToken.h"
#include "Keyword.h"

namespace c4 {
    namespace token {
        class KeywordToken : public IToken {
        public:
            KeywordToken(Keyword keyword);
            virtual ~KeywordToken() { }

            Keyword getKeyword();

        private:
            Keyword keyword;
        };
    }
}
