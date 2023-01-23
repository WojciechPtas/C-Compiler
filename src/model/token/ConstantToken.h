#pragma once

#include <string>

#include "../ConstantType.h"
#include "Token.h"

namespace c4 {
    namespace model {
        namespace token {
            class ConstantToken : public Token {
            public:
                const ConstantType type;
                const std::string value;

                ConstantToken(
                    TokenPosition position,
                    ConstantType type,
                    std::string value
                ) : Token(position), type(type), value(value) { }

                ~ConstantToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                virtual std::shared_ptr<Token> convertDigraph() const override {
                    return std::make_shared<ConstantToken>(*this);
                }
            };
        }
    }
}
