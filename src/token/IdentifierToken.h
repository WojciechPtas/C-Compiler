#pragma once

#include <string>
#include "IToken.h"

namespace c4 {
    namespace token {
        class IdentifierToken : public IToken {
        public:
            IdentifierToken(std::string identifier);
            virtual ~IdentifierToken() { }

            void accept(ITokenVisitor &visitor) override;

            std::string getIdentifier();

        private:
            std::string identifier;
        };
    }
}
