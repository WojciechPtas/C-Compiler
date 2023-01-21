#pragma once

namespace c4 {
    namespace model {
        namespace token {
            // In order to avoid circular references, we need the following
            // declarations.

            class ConstantToken;
            class ErrorToken;
            class IdentifierToken;
            class KeywordToken;
            class PunctuatorToken;
            class EOFToken;

            // The subject of this file.

            class ITokenVisitor {
            public:
                virtual ~ITokenVisitor() { }

                virtual void visit(const ConstantToken &token) = 0;
                virtual void visit(const ErrorToken &token) = 0;
                virtual void visit(const IdentifierToken &token) = 0;
                virtual void visit(const KeywordToken &token) = 0;
                virtual void visit(const PunctuatorToken &token) = 0;
                virtual void visit(const EOFToken &token) = 0;
            };
        }
    }
}
