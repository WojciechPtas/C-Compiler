#pragma once

namespace c4 {
    namespace token {
        class CharacterConstantToken;
        class DecimalConstantToken;
        class IdentifierToken;
        class KeywordToken;
        class PunctuatorToken;
        class StringLiteralToken;

        class ITokenVisitor {
        public:
            virtual ~ITokenVisitor() { }

            virtual void visit(CharacterConstantToken &token) const = 0;
            virtual void visit(DecimalConstantToken &token) = 0;
            virtual void visit(IdentifierToken &token) = 0;
            virtual void visit(KeywordToken &token) = 0;
            virtual void visit(PunctuatorToken &token) = 0;
            virtual void visit(StringLiteralToken &token) = 0;
        };
    }
}
