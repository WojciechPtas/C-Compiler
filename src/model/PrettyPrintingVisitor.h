#pragma once
#include <ostream>
#include "IASTVisitor.h"
#include "statement/CompoundStatement.h"
#include "statement/ExpressionStatement.h"
#include "statement/IterationStatement.h"
#include "statement/JumpStatement.h"
#include "statement/LabeledStatement.h"
#include "statement/SelectionStatement.h"
#include "declaration/Declaration.h"
#include "declaration/DeclarationSpecifier.h"
#include "declaration/Declarator.h"
#include "declaration/DirectDeclarator.h"
#include "declaration/DirectDeclarator2.h"
#include "declaration/FunctionDefinition.h"
#include "declaration/ParameterTypeList.h"
#include "declaration/Pointer.h"
#include "declaration/Root.h"
#include "declaration/StructDeclarationList.h"
#include "declaration/StructUnionSpecifier.h"
#include "declaration/ParameterDeclaration.h"


namespace c4::model::statement
{
    class PrettyPrintinVisitor : public IASTVisitor{
        void printIndentation();
        std::ostream& os;
        int indentation;
        bool afterWhile;
        bool afterIf;
        bool exitingCompound;
        bool afterElse;
        public:
        PrettyPrintinVisitor(std::ostream& os) : os(os),  indentation(0), 
         afterWhile(false), afterIf(false),exitingCompound(false), afterElse(false)
        {}
        void visit(const CompoundStatement& s)override;
        void visit(const ExpressionStatement& s)override;
        void visit(const IterationStatement& s)override;
        void visit(const JumpStatement& s)override;
        void visit(const LabeledStatement& s)override;
        void visit(const SelectionStatement& s)override;
        void visit(const declaration::Declaration& s)override;
        void visit(const declaration::DeclarationSpecifier & s)override;
        void visit(const declaration::Declarator& s)override;
        void visit(const declaration::DirectDeclarator& s)override;
        void visit(const declaration::DirectDeclarator2& s)override;
        void visit(const declaration::FunctionDefinition& s)override;
        void visit(const declaration::ParameterDeclaration& s)override;
        void visit(const declaration::ParameterTypeList& s)override;
        void visit(const declaration::Pointer& s)override;
        void visit(const declaration::Root & s)override;
        void visit(const declaration::StructDeclarationList & s)override;
        void visit(const declaration::StructUnionSpecifier & s)override; 
    };
} // namespace c4::model::statement
