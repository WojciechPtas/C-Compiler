#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace token {
        class StringLiteralToken : Token {
        public:
            StringLiteralToken(TokenPosition position, std::string value);
            virtual ~StringLiteralToken() { }

            void accept(ITokenVisitor &visitor) override;

            std::string getValue();

        private:
            std::string value;
        };
    }
}
