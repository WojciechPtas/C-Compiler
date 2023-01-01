#include <stdexcept>

#include "../../model/ConstantType.h"
#include "../../model/expression/BinaryExpression.h"
#include "../../model/expression/ConditionalExpression.h"
#include "../../model/expression/ConstantExpression.h"
#include "../../model/expression/IdentifierExpression.h"
#include "../../model/expression/IndexExpression.h"
#include "../../model/expression/MemberExpression.h"
#include "../../model/expression/UnaryExpression.h"

#include "BinaryExpressionUtilities.h"
#include "MemberExpressionUtilities.h"
#include "PrintVisitor.h"
#include "UnaryExpressionUtilities.h"

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

void PrintVisitor::visit(const CallExpression &expr) {
    // TODO
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
    expr.container->accept(*this);
    this->outputStream << "[";
    expr.index->accept(*this);
    this->outputStream << "]";
}

void PrintVisitor::visit(const MemberExpression &expr) {
    expr.container->accept(*this);
    this->outputStream << stringify(expr.type);
    expr.member->accept(*this);
}

void PrintVisitor::visit(const UnaryExpression &expr) {
    this->outputStream << stringify(expr.type);
    expr.expression->accept(*this);
}
