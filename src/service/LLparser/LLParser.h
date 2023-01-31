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
#include "../../util/token/PrintVisitor.h"

#include "../io/IBufferedInputStream.h"
#include "../io/DelimiterInputStream.h"
#include "../io/ParenthesisDelimiterStream.h"

#include "../../model/statement/ExpressionStatement.h"
#include "../../model/statement/CompoundStatement.h"
#include "../../model/statement/JumpStatement.h"
#include "../../model/statement/IterationStatement.h"
#include "../../model/statement/LabeledStatement.h"
#include "../../model/statement/SelectionStatement.h"

#include "../../model/declaration/FunctionDefinition.h"
#include "../../model/declaration/Root.h"

namespace c4::service::parser{
    
    class LLParser{
        private:
            std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> m_input;
            util::token::ParserVisitor visitor;
            std::shared_ptr<model::token::Token> token;
            int errorcode;
        public:
            LLParser(std::shared_ptr<io::IBufferedInputStream<std::shared_ptr<model::token::Token>>> input) : m_input(input){
                visitor=c4::util::token::ParserVisitor();
                token=nullptr;
                errorcode=0;
                };
            int run();
            std::shared_ptr<model::declaration::Root> parse(/* io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input*/);
        private:
            bool consume(util::token::TokenKind k, util::token::SpecifiedToken s=util::token::SpecifiedToken(), bool inlookahead=false);
            bool checkLookAhead(util::token::TokenKind k, util::token::SpecifiedToken s=util::token::SpecifiedToken());
            std::shared_ptr<model::declaration::Declaration> parseDeclaration();
            //std::shared_ptr<> parseStaticAssertDeclaration();
            std::shared_ptr<model::declaration::StructUnionSpecifier> parseStructorUnionSpecifier();
            std::shared_ptr<model::declaration::StructDeclarationList> parseStructDeclarationList();
            std::shared_ptr<model::declaration::Pointer> parsePointer();
            std::shared_ptr<model::declaration::Declarator> parseDeclarator();
            std::shared_ptr<model::declaration::Declarator> parseAbstractDeclarator();
            std::shared_ptr<model::declaration::DirectDeclarator> parseDirectAbstractDeclarator();
            std::shared_ptr<model::declaration::DirectDeclarator2> parseDirectAbstractDeclarator2();
            std::shared_ptr<model::declaration::DirectDeclarator> parseDirectDeclarator();
            std::shared_ptr<model::declaration::DirectDeclarator2> parseDirectDeclarator2();
            std::shared_ptr<model::declaration::ParameterTypeList> parseParameterTypeList();
            std::shared_ptr<model::declaration::ParameterDeclaration> parseParameterDeclaration();
            std::shared_ptr<model::declaration::DeclarationSpecifier> parseDeclarationSpecifier();
            std::shared_ptr<model::statement::CompoundStatement> parseCompoundStatement();
            std::shared_ptr<model::statement::SelectionStatement> parseSelectionStatement();
            std::shared_ptr<model::statement::IterationStatement> parseIterationStatement();
            std::shared_ptr<model::statement::JumpStatement> parseJumpStatement();
            std::shared_ptr<model::statement::IStatement> parseStatement();
            std::shared_ptr<model::statement::LabeledStatement> parseLabeledStatement();
            bool visit();
    };
}
