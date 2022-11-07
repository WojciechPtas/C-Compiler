#pragma once

#include "IToken.h"
#include "Punctuator.h"

namespace c4 {
    namespace token {
        class PunctuatorToken {
        public:
            PunctuatorToken(Punctuator punctuator);
            virtual ~PunctuatorToken() { }

            Punctuator getPunctuator();

        private:
            Punctuator punctuator;
        };
    }
}
