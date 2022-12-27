#pragma once

#include "ITokenVisitor.h"
#include "TokenPosition.h"

namespace c4 {
    namespace model {
        namespace token {
            class Token {
            public:
                const TokenPosition position;

                virtual ~Token() { }
                virtual void accept(ITokenVisitor &visitor) const = 0;

            protected:
                Token(TokenPosition position) : position(position) { }
            };
        }
    }
}
