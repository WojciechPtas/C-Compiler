#pragma once
#include <ostream>
#include <map>
#include <vector>
#include <set>
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


namespace c4::util::sema
{
    enum class simple_type{_VOID,_INT,_CHAR,_STRUCT};
    struct type{
        simple_type simple;
        int ptr_num=0;
        //
    };
    class SemanticalAnalyser : public IASTVisitor{
        std::ostream& os;
        bool errorFound;
        int inLoop;
        bool inParameter;
        std::map<std::string,bool> labels;
        std::map<std::string, std::shared_ptr<const model::token::Token>> gotoLabels;
        std::vector<std::set<std::string>> declared_vars;
        std::vector<std::map<std::string,type>> variables_type;
        type being_built;
        bool isVar;
        std::string name;
        bool ahead;
        void reportError(std::shared_ptr<const model::token::Token> t, std::string msg="wrong token");
        public:
        SemanticalAnalyser(std::ostream& os) : os(os),  errorFound(false), inLoop(0), inParameter(false), isVar(true), name(""),ahead(false)
        {
            declared_vars.push_back(std::set<std::string>());
            variables_type.push_back(std::map<std::string,type>());
        } //global range
        bool isErrorFound(){return errorFound;}
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
