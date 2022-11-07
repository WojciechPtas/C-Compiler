#pragma once

#include "../../util/token/ITokenVisitor.h"
#include "TokenPosition.h"

namespace c4 {
    namespace model {
        namespace token {
            class Token {
            public:
                virtual ~Token() { }

                virtual void accept(util::token::ITokenVisitor &visitor) = 0;

                const TokenPosition &getPosition();

            protected:
                Token(TokenPosition position);

            private:
                TokenPosition position;
            };
        }
    }
}
