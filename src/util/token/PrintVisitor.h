#pragma once

#include <ostream>
#include "ITokenVisitor.h"

namespace c4 {
    namespace util {
        namespace token {
            class PrintVisitor : public ITokenVisitor {
            public:
                PrintVisitor(std::ostream &outputStream);
                virtual ~PrintVisitor() { }

                void visit(
                    model::token::CharacterConstantToken &token
                ) override;

                void visit(
                    model::token::DecimalConstantToken &token
                ) override;

                void visit(model::token::IdentifierToken &token) override;
                void visit(model::token::KeywordToken &token) override;
                void visit(model::token::PunctuatorToken &token) override;
                void visit(model::token::StringLiteralToken &token) override;

            private:
                std::ostream &outputStream;

                void printPosition(model::token::Token &token);
            };
        }
    }
}
