#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace token {
        class IdentifierToken : public Token {
        public:
            IdentifierToken(TokenPosition position, std::string identifier);
            virtual ~IdentifierToken() { }

            void accept(ITokenVisitor &visitor) override;

            std::string &getIdentifier();

        private:
            std::string identifier;
        };
    }
}
