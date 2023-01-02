#include "../../model/token/CharacterConstantToken.h"
#include "../../model/token/DecimalConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"
#include "../../model/token/StringLiteralToken.h"
#include "KeywordUtilities.h"
#include "ParserVisitor.h"
#include "PunctuatorUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

TokenKind ParserVisitor::visit(const CharacterConstantToken &token) {
    return TokenKind::character_constant;
}


TokenKind ParserVisitor::visit(const DecimalConstantToken &token) {
    return TokenKind::constant;
}

TokenKind ParserVisitor::visit(const ErrorToken &token) {
   return TokenKind::Error;
}

TokenKind ParserVisitor::visit(const IdentifierToken &token) {
    return TokenKind::identifier;
}

TokenKind ParserVisitor::visit(const KeywordToken &token) {
    return keyword;
}

TokenKind ParserVisitor::visit(const PunctuatorToken &token) {
    return TokenKind::punctuator;
}

TokenKind ParserVisitor::visit(const StringLiteralToken &token) {
    return string_literal;
}