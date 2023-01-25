#pragma once
#include <string>
#include <ostream>
#include "../../model/token/ITokenVisitor.h"
#include "TokenKind.h"
namespace c4::util::token{
    class ParserVisitor : public model::token::ITokenVisitor {
        TokenKind kind;
        SpecifiedToken specifed;
        std::string val;
        public:
        ParserVisitor(){}

        void visit(
            const model::token::ConstantToken &token
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
            const model::token::EOFToken &token
        ) override;
        TokenKind getKind(){return kind;}
        SpecifiedToken getSepcificValue() {return specifed;}
        std::string getVal(){return val;}
    };
}