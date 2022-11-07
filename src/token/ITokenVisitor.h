#pragma once

#include "CharacterConstantToken.h"
#include "DecimalConstantToken.h"
#include "IdentifierToken.h"
#include "KeywordToken.h"
#include "PunctuatorToken.h"
#include "StringLiteralToken.h"

namespace c4 {
    namespace token {
        class ITokenVisitor {
        public:
            virtual ~ITokenVisitor() { }

            virtual void visit(CharacterConstantToken &token) const = 0;
            virtual void visit(DecimalConstantToken &token) = 0;
            virtual void visit(IdentifierToken &token) = 0;
            virtual void visit(KeywordToken &token) = 0;
            virtual void visit(PunctuatorToken &token) = 0;
            virtual void visit(StringLiteralToken &token) = 0;
        };
    }
}
