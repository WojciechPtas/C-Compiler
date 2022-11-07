#pragma once

#include "IToken.h"

namespace c4 {
    namespace token {
        class ConstantToken : public IToken {
        public:
            virtual ~ConstantToken() { }
        };
    }
}
