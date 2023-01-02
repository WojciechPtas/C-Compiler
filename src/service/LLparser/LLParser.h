#pragma once
#include <memory>
#include <vector>

#include "../../model/token/Token.h"
#include "../../util/token/ParserVisitor.h"
#include "../io/IInputStream.h"
namespace c4::service::parser{
    class LLParser{
        private:
            io::IInputStream<std::shared_ptr<const model::token::Token>> &m_input
        public:
            LLParser();
            bool parse( io::IInputStream<std::shared_ptr<const model::token::Token>> &input);
        private:
            bool parseDeclaration();
            bool parseStaticAssertDeclaration();
            bool parseStructorUnionSpecifier();

    }
}