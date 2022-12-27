#include <stdexcept>

#include "../../model/token/ConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"
#include "KeywordUtilities.h"
#include "PrintVisitor.h"
#include "PunctuatorUtilities.h"

using namespace c4::model;
using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

PrintVisitor::PrintVisitor(ostream &outputStream)
    : outputStream(outputStream) {
}

void PrintVisitor::visit(const ConstantToken &token) {
    this->printPosition(token);

    switch (token.type) {
    case ConstantType::Character:
    case ConstantType::Decimal:
        this->outputStream << "constant ";
        this->outputStream << token.value;
        this->outputStream << "\n";
        break;

    case ConstantType::String:
        this->outputStream << "string-literal \"";
        this->outputStream << token.value;
        this->outputStream << "\"\n";
        break;

    default:
        throw logic_error("Unimplemented constant token type");
    }
}


void PrintVisitor::visit(const ErrorToken &token) {
    this->printPosition(token);

    this->outputStream << "error: ";
    this->outputStream << token.message;
    this->outputStream << "\n";
}

void PrintVisitor::visit(const IdentifierToken &token) {
    this->printPosition(token);

    this->outputStream << "identifier ";
    this->outputStream << token.identifier;
    this->outputStream << "\n";
}

void PrintVisitor::visit(const KeywordToken &token) {
    this->printPosition(token);

    this->outputStream << "keyword ";
    this->outputStream << stringify(token.keyword);
    this->outputStream << "\n";
}

void PrintVisitor::visit(const PunctuatorToken &token) {
    this->printPosition(token);

    this->outputStream << "punctuator ";
    this->outputStream << stringify(token.punctuator);
    this->outputStream << "\n";
}

void PrintVisitor::printPosition(const Token &token) {
    auto pos = token.position;

    this->outputStream << pos.file;
    this->outputStream << ":";
    this->outputStream << pos.line;
    this->outputStream << ":";
    this->outputStream << pos.column;
    this->outputStream << ": ";
}
