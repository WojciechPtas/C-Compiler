#pragma once
#include <memory>
#include <vector>
#include "../parser/ExpressionParser.h"
#include "../../model/token/ConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"
#include "../../model/token/Token.h"
#include "../../util/token/ParserVisitor.h"
#include "../io/IBufferedInputStream.h"
namespace c4::service::parser{
    
    class LLParser{
        private:
            std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> m_input;
            util::token::ParserVisitor visitor;
            std::shared_ptr<model::token::Token> token;
            std::shared_ptr<ExpressionParser> expression_parser;
        public:
            LLParser(std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> input, std::shared_ptr<ExpressionParser> expression_parser) : m_input(input), expression_parser(expression_parser){
                visitor=c4::util::token::ParserVisitor();
                token=nullptr;
                };
            bool parse(/* io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input*/);
        private:
            bool consume(util::token::TokenKind k, util::token::SpecifiedToken s, bool inlookahead=false);
            bool checkLookAhead(util::token::TokenKind k, util::token::SpecifiedToken s={.empty=true});
            bool parseDeclaration();
            bool parseStaticAssertDeclaration();
            bool parseStructorUnionSpecifier();
            bool parseStructDeclarationList();
            bool parsePointer();
            bool parseDeclarator();
            bool parseDirectDeclarator();
            bool parseDirectDeclarator2();
            bool parseParameterTypeList();
            bool parseParameterDeclaration();
            bool parseDeclarationSpecifier();
            bool parseIdentifierList();
            bool parseCompoundStatement();
            bool parseSelectionStatement();
            bool parseIterationStatement();
            bool parseJumpStatement();
            bool parseStatement();
            bool parseLabeledStatement();
            bool visit();
    };
}
