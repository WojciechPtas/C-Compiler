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
#include "../io/DelimiterInputStream.h"
#include "../io/ParenthesisDelimiterStream.h"


#include "../../model/statement/CompoundStatement.h"
#include "../../model/statement/JumpStatement.h"
#include "../../model/declaration/IDeclaration.h"
#include "../../model/statement/IterationStatement.h"
#include "../../model/statement/LabeledStatement.h"
#include "../../model/statement/SelectionStatement.h"

namespace c4::service::parser{
    
    class LLParser{
        private:
            std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> m_input;
            util::token::ParserVisitor visitor;
            std::shared_ptr<model::token::Token> token;
        public:
            LLParser(std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> input) : m_input(input){
                visitor=c4::util::token::ParserVisitor();
                token=nullptr;
                };
            bool parse(/* io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input*/);
        private:
            bool consume(util::token::TokenKind k, util::token::SpecifiedToken s=util::token::SpecifiedToken(), bool inlookahead=false);
            bool checkLookAhead(util::token::TokenKind k, util::token::SpecifiedToken s=util::token::SpecifiedToken());
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
