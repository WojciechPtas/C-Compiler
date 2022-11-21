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

                void accept(
                    util::token::ITokenVisitor &visitor
                ) const override;

                const std::string &getValue() const;

            private:
                const std::string value;
            };
        }
    }
}
