#include "../../model/token/CharacterConstantToken.h"
#include "../../model/token/DecimalConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"
#include "../../model/token/StringLiteralToken.h"
#include "KeywordUtilities.h"
#include "PrintVisitor.h"
#include "PunctuatorUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

PrintVisitor::PrintVisitor(ostream &outputStream)
    : outputStream(outputStream) {
}

void PrintVisitor::visit(const CharacterConstantToken &token) {
    this->printPosition(token);

    this->outputStream << "constant ";
    this->outputStream << token.getValue();
    this->outputStream << "\n";
}


void PrintVisitor::visit(const DecimalConstantToken &token) {
    this->printPosition(token);

    this->outputStream << "constant ";
    this->outputStream << token.getValue();
    this->outputStream << "\n";
}

void PrintVisitor::visit(const ErrorToken &token) {
    this->printPosition(token);

    this->outputStream << "error: ";
    this->outputStream << token.getMessage();
    this->outputStream << "\n";
}

void PrintVisitor::visit(const IdentifierToken &token) {
    this->printPosition(token);

    this->outputStream << "identifier ";
    this->outputStream << token.getIdentifier();
    this->outputStream << "\n";
}

void PrintVisitor::visit(const KeywordToken &token) {
    this->printPosition(token);

    this->outputStream << "keyword ";
    this->outputStream << stringify(token.getKeyword());
    this->outputStream << "\n";
}

void PrintVisitor::visit(const PunctuatorToken &token) {
    this->printPosition(token);

    this->outputStream << "punctuator ";
    this->outputStream << stringify(token.getPunctuator());
    this->outputStream << "\n";
}

void PrintVisitor::visit(const StringLiteralToken &token) {
    this->printPosition(token);

    this->outputStream << "string-literal \"";
    this->outputStream << token.getValue();
    this->outputStream << "\"\n";
}

void PrintVisitor::printPosition(const Token &token) {
    auto pos = token.getPosition();

    this->outputStream << pos.getFile();
    this->outputStream << ":";
    this->outputStream << pos.getLine();
    this->outputStream << ":";
    this->outputStream << pos.getColumn();
    this->outputStream << ": ";
}
