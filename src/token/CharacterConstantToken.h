#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace token {
        class CharacterConstantToken : public ConstantToken {
        public:
            CharacterConstantToken(TokenPosition position, std::string value);
            virtual ~CharacterConstantToken() { }

            void accept(ITokenVisitor &visitor) override;

            const std::string &getValue();

        private:
            std::string value;
        };
    }
}
