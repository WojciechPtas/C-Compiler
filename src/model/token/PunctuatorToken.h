#pragma once

#include "Token.h"
#include "Punctuator.h"

namespace c4 {
    namespace model {
        namespace token {
            class PunctuatorToken : public Token {
            public:
                const Punctuator punctuator;

                PunctuatorToken(
                    TokenPosition position,
                    Punctuator punctuator
                ) : Token(position), punctuator(punctuator) { }

                ~PunctuatorToken() { }

                void accept(ITokenVisitor &visitor) const override {
                    visitor.visit(*this);
                }

                virtual std::shared_ptr<Token> convertDigraph() const override {
                    std::shared_ptr<Token> converted;
                    switch(punctuator) {
                        case Punctuator::LessThanColon:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position, 
                                Punctuator::LeftBracket
                            );
                            break;
                        case Punctuator::ColonGreaterThan:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position, 
                                Punctuator::RightBracket
                            );
                            break;
                        case Punctuator::LessThanPercent:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position, 
                                Punctuator::LeftBrace
                            );
                            break;
                        case Punctuator::PercentGreaterThan:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position,
                                Punctuator::RightBrace
                            );
                            break;
                        case Punctuator::PercentColon:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position,
                                Punctuator::Hashtag
                            );
                            break;
                        case Punctuator::PercentColonPercentColon:
                            converted = std::make_shared<PunctuatorToken>(
                                this->position, 
                                Punctuator::DoubleHashtag
                            );
                            break;
                        default:
                            converted = std::make_shared<PunctuatorToken>(*this);
                    }
                    return converted;
                }
            };
        }
    }
}
