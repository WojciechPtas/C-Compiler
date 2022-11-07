#pragma once

#include "ITokenVisitor.h"
#include "TokenPosition.h"

namespace c4 {
    namespace token {
        class Token {
        public:
            virtual ~Token() { }

            virtual void accept(ITokenVisitor &visitor) = 0;

            const TokenPosition &getPosition();

        protected:
            Token(TokenPosition position);

        private:
            TokenPosition position;
        };
    }
}
