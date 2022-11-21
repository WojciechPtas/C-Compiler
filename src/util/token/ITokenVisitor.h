#pragma once

namespace c4 {
    namespace model {
        namespace token {
            class CharacterConstantToken;
            class DecimalConstantToken;
            class ErrorToken;
            class IdentifierToken;
            class KeywordToken;
            class PunctuatorToken;
            class StringLiteralToken;
        }
    }

    namespace util {
        namespace token {
            class ITokenVisitor {
            public:
                virtual ~ITokenVisitor() { }

                virtual void visit(
                    const model::token::CharacterConstantToken &token
                ) = 0;

                virtual void visit(
                    const model::token::DecimalConstantToken &token
                ) = 0;

                virtual void visit(const model::token::ErrorToken &token) = 0;
                virtual void visit(
                    const model::token::IdentifierToken &token
                ) = 0;

                virtual void visit(
                    const model::token::KeywordToken &token
                ) = 0;

                virtual void visit(
                    const model::token::PunctuatorToken &token
                ) = 0;
                
                virtual void visit(
                    const model::token::StringLiteralToken &token
                ) = 0;
            };
        }
    }
}
