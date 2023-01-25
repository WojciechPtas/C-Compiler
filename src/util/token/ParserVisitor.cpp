#include "../../model/token/ConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"

#include "./KeywordUtilities.h"
#include "./ParserVisitor.h"
#include "./PunctuatorUtilities.h"
#include <iostream>
using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

void ParserVisitor::visit(const model::token::ConstantToken &token) {
    this->kind= TokenKind::character_constant;
    auto a =token;
    //std::cout<< "ConstantToken!\n";
    
}


// void ParserVisitor::visit(const DecimalConstantToken &token) {
//     this->kind= TokenKind::constant;
// }

void ParserVisitor::visit(const ErrorToken &token) {
   this->kind= TokenKind::error;
    auto a =token;
    //std::cout<< "ErrorToken!\n";

}

void ParserVisitor::visit(const IdentifierToken &token) {
    this->kind= TokenKind::identifier;
    auto a =token;
    val=token.identifier;
    //std::cout<< "IdentifierToken!\n";

}

void ParserVisitor::visit(const KeywordToken &token) {
    auto a =token;
    
    //std::cout<< "KeywordToken!\n";
    this->kind = TokenKind::keyword;
    this->specifed = SpecifiedToken( token.keyword);
}

void ParserVisitor::visit(const PunctuatorToken &token) {
    auto a =token;
    this->kind= TokenKind::punctuator;
    this->specifed = SpecifiedToken(token.punctuator);
    //std::cout<< "PunctuatorToken\n";

}

void ParserVisitor::visit(const model::token::EOFToken &token) {
    (void) token;
    this->kind = TokenKind::eof;
}

// void ParserVisitor::visit(const StringLiteralToken &token) {
//     this->kind= TokenKind::string_literal;
// }