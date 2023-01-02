#pragma once

#include <ostream>
#include "ITokenVisitor.h"
#include "TokenKind.h"
namespace c4::util::token{
    class ParserVisitor : public ITokenVisitor{
        TokenKind kind;
        public:
        ParserVisitor(){}
        void visit(
            const model::token::CharacterConstantToken &token
        ) override;
        void visit(
            const model::token::DecimalConstantToken &token
        ) override;
        void visit(const model::token::ErrorToken &token) override;
        void visit(
            const model::token::IdentifierToken &token
        ) override;
        void visit(const model::token::KeywordToken &token) override;
        void visit(
            const model::token::PunctuatorToken &token
        ) override;
        void visit(
            const model::token::StringLiteralToken &token
        ) override;
        TokenKind getKind(){return kind;}
    };
}