#include "PrettyPrintingVisitor.h"
#include <iostream>
#include "../../expression/PrintVisitor.h"
using namespace c4::util::pretty;
using namespace c4::model::statement;
using namespace c4::model::declaration;
using namespace c4::util::expression;
void c4::util::pretty::PrettyPrintinVisitor::printIndentation()
{
    for(int i=0;i<indentation;i++){
        os << "\t";
    }
}
void PrettyPrintinVisitor::visit(const CompoundStatement &s)
{
    if(afterWhile){
        afterIf=false;
        afterElse=false;
        afterWhile=false;
        os<<" {\n";
        for(auto& a : s.block_of_statements){
            a->accept(*this);
        }
        indentation--;
        printIndentation();
        indentation++;
        os<<"}\n";
    }
    else if(afterIf){
        afterIf=false;
        afterElse=false;
        afterWhile=false;
        os<<" {\n";
        for(auto& a : s.block_of_statements){
            a->accept(*this);
        }
        indentation--;
        printIndentation();
        indentation++;
        os<<"}";
    }
    else if(afterElse){
        afterIf=false;
        afterElse=false;
        afterWhile=false;
        os<<" {\n";
        for(auto& a : s.block_of_statements){
            a->accept(*this);
        }
        indentation--;
        printIndentation();
        indentation++;
        os<<"}\n";
    }
    else{
    printIndentation();
    os<<"{\n";
    indentation++;
    afterIf=false;
    afterElse=false;
    afterWhile=false;
    for(auto& a : s.block_of_statements){
        a->accept(*this);
    }
    indentation--;
    printIndentation();
    os<<"}\n";
    }
    exitingCompound=true;
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const ExpressionStatement &s)
{
    if(afterElse||afterWhile||afterIf) os<<"\n";
    afterElse=false;
    afterWhile=false;
    afterIf=false;
    printIndentation();
    PrintVisitor p(os);
    if(s.expr!=nullptr)
    s.expr->accept(p);
    os << ";\n";
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const IterationStatement &s)
{
    if(afterElse||afterWhile||afterIf) os<<"\n";
    afterElse=false;
    afterWhile=false;
    afterIf=false;
    printIndentation();
    os<<"while (";
    PrintVisitor p(os);
    s.expr->accept(p);
    os<<")";
    afterWhile=true;
    indentation++;
    if(s.statement!=nullptr)
    s.statement->accept(*this);
    indentation--;
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const JumpStatement &s)
{
    if(afterElse||afterWhile||afterIf) os<<"\n";
    afterElse=false;
    afterWhile=false;
    afterIf=false;
    printIndentation();
    switch(s.k){
        case kind::_break:
        os << "break;\n";
        break;
        case kind::_continue:
        os << "continue;\n";
        break;
        case kind::_goto:
        os <<"goto "<< s.gotoIdentifier <<";\n";
        break;
        case kind::_return:
        os << "return"; 
        PrintVisitor p(os);
        if(s.returnExpression!=nullptr){
            os<<" ";
            s.returnExpression->accept(p);
        }
        
        os << ";\n";
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const LabeledStatement &s)
{
    if(afterElse||afterWhile||afterIf) os<<"\n";
    afterElse=false;
    afterWhile=false;
    afterIf=false;
    os << s.identifier << ":\n";
    if(s.statement!=nullptr)
    s.statement->accept(*this);
    
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const SelectionStatement &s)
{
    if(afterIf) os <<"\n";
    auto a=afterElse;
    if(!a)
    printIndentation();
    else{
        os<<" ";
    }
    os<<"if (";
    PrintVisitor p(os);
    s.ifExpr->accept(p);
    os <<")";
    afterIf=true;
    if(!a){
        indentation++;
    }
    if(s.thenStatement!=nullptr)
    s.thenStatement->accept(*this);
    afterIf=false;

    if(s.elseStatement!=nullptr){
        if(!exitingCompound){
        indentation--;
        printIndentation();
        indentation++;
        }
        else{
            os<<" ";
        }
        exitingCompound=false;
        os<<"else";
        afterElse=true;
        s.elseStatement->accept(*this);
    }
    else{
        if(!exitingCompound)
        exitingCompound=false;
        os<<"\n";
    }
    if(!a){
        indentation--;
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const Declaration &s)
{
    if(afterElse||afterWhile||afterIf) os<<"\n";
    afterElse=false;
    afterWhile=false;
    afterIf=false;
    printIndentation();
    s.ds->accept(*this);
    
    if(s.declarator!=nullptr){
        os<<" ";
        s.declarator->accept(*this);
    }
    os<<";\n";
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const DeclarationSpecifier &s)
{
    if(s.structorunion==nullptr){
        switch(s.keyword){
            case model::token::Keyword::Int:
            os<<"int";
            break;
            case model::token::Keyword::Char:
            os<<"char";
            break;
            case model::token::Keyword::Void:
            os<<"void";
            break;
            default:
            break;
        }
    }
    else{
        s.structorunion->accept(*this);
        
    }
    //os<<"";
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const Declarator &s)
{
    bool complex=false;
    if(s.ptr!=nullptr){
        os<<"(";
        s.ptr->accept(*this);
        complex=true;
    }
    if(s.dec!=nullptr){
        s.dec->accept(*this);
    }
    for(int i=0;i<ptr;i++){
        os<<")";
    }
    ptr=0;
    if(complex){
        os<<")";
    }
}
void c4::util::pretty::PrettyPrintinVisitor::visit(const DirectDeclarator &s)
{
    ahead=true;
    if(s.direct_declarator!=nullptr){
        s.direct_declarator->accept(*this); 
    }
    else{
        ahead=false;
    }
    bool complex=ahead;
    ahead=false;
    if(complex){
        os<<"(";
    }
    if(s.declarator==nullptr){
        os<<s.identifier;
    }
    else{
        //os<<"(";
        s.declarator->accept(*this);
        //os<<")";
    }
    if(s.direct_declarator!=nullptr)
    s.direct_declarator->accept(*this);
    if(complex){
        os<<")";
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const DirectDeclarator2 &s)
{
    if(ahead){
        ahead = s.list!=nullptr || s.declarator!=nullptr;
        return;
    }
    if(s.list!=nullptr){
        os<<"(";
        s.list->accept(*this);
        os<<")";
    }
    if(s.declarator!=nullptr){
        s.declarator->accept(*this);
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const FunctionDefinition &s)
{
    printIndentation();
    s.ds->accept(*this);
    os<<" ";
    s.declarator->accept(*this);
    os<<"\n";
    s.statement->accept(*this);
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const ParameterDeclaration &s)
{
    s.type->accept(*this);
    //std::cout<<"parameter\n";
    if(s.dec!=nullptr){
        os<<" ";
        s.dec->accept(*this);
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const ParameterTypeList &s)
{
    if(s.params.size()==0) return;
    for(size_t i=0;i<s.params.size()-1;i++)
    {
        s.params[i]->accept(*this);
        os<<", ";
    }
    s.params[s.params.size()-1]->accept(*this);
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const Pointer &s)
{
    os<<"*";
    if(s.ptr!=nullptr){
        os<<"(";
        ptr++;
        s.ptr->accept(*this);
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const Root &s)
{
    for(size_t i =0; i<s.definitions.size()-1;i++){
        s.definitions[i]->accept(*this);
        os<<"\n";
    }
    s.definitions[s.definitions.size()-1]->accept(*this);
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const StructDeclarationList &s)
{
    for(auto& a: s.declarations){
        a->accept(*this);
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const StructUnionSpecifier &s)
{
    os<<"struct ";
    os<<s.name;
    if(s.declarations!=nullptr){
    os<<"\n";
    printIndentation();

    os<<"{\n";
    indentation++;
    
    s.declarations->accept(*this);
    indentation--;

    printIndentation();
    os << "}";
    }
}

void c4::util::pretty::PrettyPrintinVisitor::visit(const model::declaration::TypeName &s)
{
    s.ds->accept(*this);
    
    if(s.declarator!=nullptr){
        os<<" ";
        s.declarator->accept(*this);
    }
    return;
}
