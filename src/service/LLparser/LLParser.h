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
            io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &m_input;
        public:
            LLParser();
            bool parse( io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input);
        private:
            bool parseDeclaration();
            bool parseStaticAssertDeclaration();
            bool parseStructorUnionSpecifier();

    };
}
