#pragma once

#include <ostream>
#include "../../model/token/ITokenVisitor.h"

namespace c4 {
    namespace util {
        namespace token {
            class PrintVisitor : public model::token::ITokenVisitor {
            public:
                PrintVisitor(std::ostream &outputStream);
                virtual ~PrintVisitor() { }

                void visit(const model::token::ConstantToken &token) override;
                void visit(const model::token::ErrorToken &token) override;
                void visit(
                    const model::token::IdentifierToken &token
                ) override;

                void visit(const model::token::KeywordToken &token) override;
                void visit(
                    const model::token::PunctuatorToken &token
                ) override;

            private:
                std::ostream &outputStream;

                void printPosition(const model::token::Token &token);
            };
        }
    }
}
