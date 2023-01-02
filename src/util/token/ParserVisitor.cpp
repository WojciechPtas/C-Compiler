#include "../../model/token/ConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"

#include "./KeywordUtilities.h"
#include "./ParserVisitor.h"
#include "./PunctuatorUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

void ParserVisitor::visit(const model::token::ConstantToken &token) {
    this->kind= TokenKind::character_constant;
}


// void ParserVisitor::visit(const DecimalConstantToken &token) {
//     this->kind= TokenKind::constant;
// }

void ParserVisitor::visit(const ErrorToken &token) {
   this->kind= TokenKind::error;
}

void ParserVisitor::visit(const IdentifierToken &token) {
    this->kind= TokenKind::identifier;
}

void ParserVisitor::visit(const KeywordToken &token) {
    this->kind= TokenKind::keyword;
}

void ParserVisitor::visit(const PunctuatorToken &token) {
    this->kind= TokenKind::punctuator;
}

// void ParserVisitor::visit(const StringLiteralToken &token) {
//     this->kind= TokenKind::string_literal;
// }