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
    class TypeName;
}


namespace c4::model::statement
{
    class CompoundStatement;
    class ExpressionStatement;
    class IterationStatement;
    class JumpStatement;
    class LabeledStatement;
    class SelectionStatement;
}
namespace c4::util{
    class IASTVisitor{

        public:
        virtual void visit(const model::statement::ExpressionStatement& s)=0;
        virtual void visit(const model::statement::CompoundStatement& s)=0;
        virtual void visit(const model::statement::IterationStatement& s)=0;
        virtual void visit(const model::statement::JumpStatement& s)=0;
        virtual void visit(const model::statement::LabeledStatement& s)=0;
        virtual void visit(const model::statement::SelectionStatement& s)=0;
        virtual void visit(const model::declaration::Declaration& s)=0;
        virtual void visit(const model::declaration::DeclarationSpecifier & s)=0;
        virtual void visit(const model::declaration::Declarator& s)=0;
        virtual void visit(const model::declaration::DirectDeclarator& s)=0;
        virtual void visit(const model::declaration::DirectDeclarator2& s)=0;
        virtual void visit(const model::declaration::FunctionDefinition& s)=0;
        virtual void visit(const model::declaration::ParameterDeclaration& s)=0;
        virtual void visit(const model::declaration::ParameterTypeList& s)=0;
        virtual void visit(const model::declaration::Pointer& s)=0;
        virtual void visit(const model::declaration::Root & s)=0;
        virtual void visit(const model::declaration::StructDeclarationList & s)=0;
        virtual void visit(const model::declaration::StructUnionSpecifier & s)=0; 
        virtual void visit(const model::declaration::TypeName & s) =0;

    };


} // namespace c4::model::statement
