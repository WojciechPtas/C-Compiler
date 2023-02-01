#pragma once

namespace c4::model::declaration{
    class Declaration;
    class DeclarationSpecifier;
    class Declarator;
    class DirectDeclarator;
    class DirectDeclarator2;
    class FunctionDefinition;
    class ParameterDeclaration;
    class ParameterTypeList;
    class Pointer;
    class Root;
    class StructDeclarationList;
    class StructUnionSpecifier;
}


namespace c4::model::statement
{
    class CompoundStatement;
    class ExpressionStatement;
    class IterationStatement;
    class JumpStatement;
    class LabeledStatement;
    class SelectionStatement;
    
    class IASTVisitor{

        public:
        virtual void visit(const CompoundStatement& s)=0;
        virtual void visit(const ExpressionStatement& s)=0;
        virtual void visit(const IterationStatement& s)=0;
        virtual void visit(const JumpStatement& s)=0;
        virtual void visit(const LabeledStatement& s)=0;
        virtual void visit(const SelectionStatement& s)=0;
        virtual void visit(const declaration::Declaration& s)=0;
        virtual void visit(const declaration::DeclarationSpecifier & s)=0;
        virtual void visit(const declaration::Declarator& s)=0;
        virtual void visit(const declaration::DirectDeclarator& s)=0;
        virtual void visit(const declaration::DirectDeclarator2& s)=0;
        virtual void visit(const declaration::FunctionDefinition& s)=0;
        virtual void visit(const declaration::ParameterDeclaration& s)=0;
        virtual void visit(const declaration::ParameterTypeList& s)=0;
        virtual void visit(const declaration::Pointer& s)=0;
        virtual void visit(const declaration::Root & s)=0;
        virtual void visit(const declaration::StructDeclarationList & s)=0;
        virtual void visit(const declaration::StructUnionSpecifier & s)=0; 

    };


} // namespace c4::model::statement
