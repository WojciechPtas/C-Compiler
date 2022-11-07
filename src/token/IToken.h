#pragma once

#include "ITokenVisitor.h"

namespace c4 {
    namespace token {
        class IToken {
        public:
            virtual ~IToken() { }

            virtual void accept(ITokenVisitor &visitor) = 0;
        };
    }
}
