#pragma once

#include <string>
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class ErrorToken : public Token {
            public:
                const std::string message;

                ErrorToken(
                    TokenPosition position,
                    std::string message
                ) : Token(position), message(message) { }

                ~ErrorToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                bool isError() const override {
                    return true;
                }

                virtual std::shared_ptr<Token> convertDigraph() const override {
                    return std::make_shared<ErrorToken>(*this);
                }
            };
        }
    }
}
