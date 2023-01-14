#pragma once
#include <memory>
#include <vector>

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
            const std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> m_input;
            util::token::ParserVisitor visitor;
            std::shared_ptr<model::token::Token> token;
        public:
            LLParser(const std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>>& input) : m_input(input){
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
            bool parseParameterTypeList();
            bool parseIdentifierList();
            bool parseCompundStatement();
            bool parseSelectionStatement();
            bool parseIterationStatement();
            bool parseJumpStatement();
            bool parseStatement();
            bool parseLabeledStatement();
            bool visit();
    };
}
