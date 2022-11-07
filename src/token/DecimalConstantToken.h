#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace token {
        class DecimalConstantToken : public ConstantToken {
        public:
            DecimalConstantToken(std::string value);
            virtual ~DecimalConstantToken() { }

            std::string getValue();

        private:
            std::string value;
        };
    }
}
