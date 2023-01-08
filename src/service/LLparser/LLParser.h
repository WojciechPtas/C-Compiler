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
    struct SpecifiedToken
    {
        bool empty;
        model::token::Punctuator p;
        model::token::Keyword k;
    };
    class LLParser{
        private:
            io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &m_input;
            util::token::ParserVisitor visitor;
            std::shared_ptr<const model::token::Token> token;
        public:
            //LLParser(){};
            bool parse( io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input);
        private:
            bool consume(util::token::TokenKind k, SpecifiedToken s);
            bool checkLookAhead(util::token::TokenKind k, SpecifiedToken s={.empty=true});
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
    };
}
