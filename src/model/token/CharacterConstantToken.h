#pragma once

#include <string>
#include "ConstantToken.h"

namespace c4 {
    namespace model {
        namespace token {
            class CharacterConstantToken : public ConstantToken {
            public:
                CharacterConstantToken(
                    TokenPosition position,
                    std::string value
                );

                virtual ~CharacterConstantToken() { }

                void accept(util::token::ITokenVisitor &visitor) override;

                const std::string &getValue();

            private:
                std::string value;
            };
        }
    }
}
