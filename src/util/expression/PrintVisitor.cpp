#include "PrintVisitor.h"

using namespace c4::model;
using namespace c4::model::expression;
using namespace c4::util::expression;
using namespace std;

PrintVisitor::PrintVisitor(ostream &outputStream) :
    outputStream(outputStream) { }

void PrintVisitor::visit(const BinaryExpression &expr) {
    this->outputStream << "(";
    expr.left->accept(*this);
    this->outputStream << " " << stringify(expr.type) << " ";
    expr.right->accept(*this);
    this->outputStream << ")";
}

void PrintVisitor::visit(const CallArguments& ca) {
    this->outputStream << "(";
    if (ca.arguments.size() > 0) {
        for (size_t i=0; i<ca.arguments.size()-1; i++) {
            ca.arguments[i]->accept(*this);
            this->outputStream << ", ";
        }
        ca.arguments[ca.arguments.size()-1]->accept(*this);
    }
    this->outputStream << ")";
}

void PrintVisitor::visit(const CallExpression &expr) {
    this->outputStream << "(";
    expr.called->accept(*this);
    expr.arguments.accept(*this);
    this->outputStream << ")";
}


void PrintVisitor::visit(const ConditionalExpression &expr) {
    this->outputStream << "(";
    expr.condition->accept(*this);
    this->outputStream << " ? ";
    expr.thenCase->accept(*this);
    this->outputStream << " : ";
    expr.elseCase->accept(*this);
    this->outputStream << ")";
}

void PrintVisitor::visit(const ConstantExpression &expr) {
    switch (expr.type) {
        case ConstantType::Character:
            this->outputStream << "'" << expr.value << "'";
            break;

        case ConstantType::Decimal:
            this->outputStream << expr.value;
            break;

        case ConstantType::String:
            this->outputStream << "\"" << expr.value << "\"";
            break;

        default:
            throw logic_error("Unimplemented constant expression type");
    }
}

void PrintVisitor::visit(const IdentifierExpression &expr) {
    this->outputStream << expr.identifier;
}

void PrintVisitor::visit(const IndexExpression &expr) {
    this->outputStream << "(";
    expr.container->accept(*this);
    this->outputStream << "[";
    expr.index->accept(*this);
    this->outputStream << "]";
    this->outputStream << ")";
}

void PrintVisitor::visit(const MemberExpression &expr) {
    this->outputStream << "(";
    expr.container->accept(*this);
    this->outputStream << stringify(expr.type);
    expr.member->accept(*this);
    this->outputStream << ")";
}

void PrintVisitor::visit(const SizeOfType &expr) {
    this->outputStream << "(sizeof("
        << token::stringify(expr.type)
        << "))";
}

void PrintVisitor::visit(const TypeInSizeof &expr) {
    (void) expr;

    this->outputStream << "(should never print)";
}

void PrintVisitor::visit(const UnaryExpression &expr) {
    this->outputStream << "(" 
        << stringify(expr.type);
    if(expr.type == UnaryExpressionType::Sizeof && expr.expression->isTerminal())
        this->outputStream << " ";
    expr.expression->accept(*this);
    this->outputStream << ")";
}


