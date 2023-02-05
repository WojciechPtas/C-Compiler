#pragma once
#include <ostream>
#include "../IASTVisitor.h"

#include "../../../model/statement/CompoundStatement.h"
#include "../../../model/statement/ExpressionStatement.h"
#include "../../../model/statement/IterationStatement.h"
#include "../../../model/statement/JumpStatement.h"
#include "../../../model/statement/LabeledStatement.h"
#include "../../../model/statement/SelectionStatement.h"
#include "../../../model/declaration/Declaration.h"
#include "../../../model/declaration/DeclarationSpecifier.h"
#include "../../../model/declaration/Declarator.h"
#include "../../../model/declaration/DirectDeclarator.h"
#include "../../../model/declaration/DirectDeclarator2.h"
#include "../../../model/declaration/FunctionDefinition.h"
#include "../../../model/declaration/ParameterTypeList.h"
#include "../../../model/declaration/Pointer.h"
#include "../../../model/declaration/Root.h"
#include "../../../model/declaration/StructDeclarationList.h"
#include "../../../model/declaration/StructUnionSpecifier.h"
#include "../../../model/declaration/ParameterDeclaration.h"

namespace c4::util::pretty
{
    class PrettyPrintinVisitor : public IASTVisitor{
        void printIndentation();
        std::ostream& os;
        int indentation;
        bool ahead;
        bool afterWhile;
        bool afterIf;
        bool exitingCompound;
        bool afterElse;
        int ptr;
        public:
        PrettyPrintinVisitor(std::ostream& os) : os(os),  indentation(0), ahead(false),
         afterWhile(false), afterIf(false),exitingCompound(false), afterElse(false), ptr(0)
        {}
        void visit(const model::statement::CompoundStatement& s)override;
        void visit(const model::statement::ExpressionStatement& s)override;
        void visit(const model::statement::IterationStatement& s)override;
        void visit(const model::statement::JumpStatement& s)override;
        void visit(const model::statement::LabeledStatement& s)override;
        void visit(const model::statement::SelectionStatement& s)override;
        void visit(const model::declaration::Declaration& s)override;
        void visit(const model::declaration::DeclarationSpecifier & s)override;
        void visit(const model::declaration::Declarator& s)override;
        void visit(const model::declaration::DirectDeclarator& s)override;
        void visit(const model::declaration::DirectDeclarator2& s)override;
        void visit(const model::declaration::FunctionDefinition& s)override;
        void visit(const model::declaration::ParameterDeclaration& s)override;
        void visit(const model::declaration::ParameterTypeList& s)override;
        void visit(const model::declaration::Pointer& s)override;
        void visit(const model::declaration::Root & s)override;
        void visit(const model::declaration::StructDeclarationList & s)override;
        void visit(const model::declaration::StructUnionSpecifier & s)override; 
    };
} // namespace c4::model::statement
