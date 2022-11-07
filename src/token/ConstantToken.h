#pragma once

#include "Token.h"

namespace c4 {
    namespace token {
        class ConstantToken : public Token {
        public:
            virtual ~ConstantToken() { }

        protected:
            ConstantToken(TokenPosition position) : Token(position) { }
        };
    }
}
