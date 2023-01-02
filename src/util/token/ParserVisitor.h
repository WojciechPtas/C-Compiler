#pragma once

#include <ostream>
#include "ITokenVisitor.h"
#include "TokenKind.h"
namespace c4::util::token{
    class ParserVisitor : public ITokenVisitor{
        TokenKind kind;
        public:
        ParserVisitor(){}
        TokenKind visit(
            const model::token::CharacterConstantToken &token
        ) override;
        TokenKind visit(
            const model::token::DecimalConstantToken &token
        ) override;
        TokenKind visit(const model::token::ErrorToken &token) override;
        TokenKind visit(
            const model::token::IdentifierToken &token
        ) override;
        TokenKind visit(const model::token::KeywordToken &token) override;
        TokenKind visit(
            const model::token::PunctuatorToken &token
        ) override;
        TokenKind visit(
            const model::token::StringLiteralToken &token
        ) override;
        TokenKind getKind(){return kind;}
    }
}