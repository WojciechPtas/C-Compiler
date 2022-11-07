#pragma once

#include "IToken.h"
#include "Keyword.h"

namespace c4 {
    namespace token {
        class KeywordToken : public IToken {
        public:
            KeywordToken(Keyword keyword);
            virtual ~KeywordToken() { }

            void accept(ITokenVisitor &visitor) override;

            Keyword getKeyword();

        private:
            Keyword keyword;
        };
    }
}
