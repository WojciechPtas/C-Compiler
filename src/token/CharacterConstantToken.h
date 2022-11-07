#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace token {
        class CharacterConstantToken : public ConstantToken {
        public:
            CharacterConstantToken(std::string value);
            virtual ~CharacterConstantToken() { }

            std::string getValue();

        private:
            std::string value;
        };
    }
}
