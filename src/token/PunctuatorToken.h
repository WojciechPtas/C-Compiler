#pragma once

#include "IToken.h"
#include "Punctuator.h"

namespace c4 {
    namespace token {
        class PunctuatorToken : public IToken {
        public:
            PunctuatorToken(Punctuator punctuator);
            virtual ~PunctuatorToken() { }

            void accept(ITokenVisitor &visitor) override;

            Punctuator getPunctuator();

        private:
            Punctuator punctuator;
        };
    }
}
