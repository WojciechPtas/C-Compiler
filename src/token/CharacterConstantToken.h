#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace token {
        class CharacterConstantToken : public ConstantToken {
        public:
            CharacterConstantToken(std::string value);
            virtual ~CharacterConstantToken() { }

            void accept(ITokenVisitor &visitor) override;

            std::string getValue();

        private:
            std::string value;
        };
    }
}
