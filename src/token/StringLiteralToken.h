#pragma once

#include <string>
#include "IToken.h"

namespace c4 {
    namespace token {
        class StringLiteralToken : IToken {
        public:
            StringLiteralToken(std::string value);
            virtual ~StringLiteralToken() { }

            void accept(ITokenVisitor &visitor) override;

            std::string getValue();

        private:
            std::string value;
        };
    }
}
