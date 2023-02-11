#pragma once

#include "../../model/ASTNode.h"
#include "../../model/expression/IExpressionCodeGenVisitor.h"
#include "../../model/expression/IExpression.h"
#include "../../model/CType/BaseCType.h"
#include "../../model/CType/CFunctionType.h"
#include "../../model/CType/ParameterInfo.h"
#include "../../model/CType/CTypedValue.h"

// AST VISITOR
#include "../../util/ASTVisitors/IASTVisitor.h"
#include "../../model/statement/CompoundStatement.h"
#include "../../model/statement/ExpressionStatement.h"
#include "../../model/statement/IterationStatement.h"
#include "../../model/statement/JumpStatement.h"
#include "../../model/statement/LabeledStatement.h"
#include "../../model/statement/SelectionStatement.h"
#include "../../model/declaration/Declaration.h"
#include "../../model/declaration/DeclarationSpecifier.h"
#include "../../model/declaration/Declarator.h"
#include "../../model/declaration/DirectDeclarator.h"
#include "../../model/declaration/DirectDeclarator2.h"
#include "../../model/declaration/FunctionDefinition.h"
#include "../../model/declaration/ParameterTypeList.h"
#include "../../model/declaration/Pointer.h"
#include "../../model/declaration/Root.h"
#include "../../model/declaration/StructDeclarationList.h"
#include "../../model/declaration/StructUnionSpecifier.h"
#include "../../model/declaration/ParameterDeclaration.h"

#include <unordered_map>
#include <iostream>
#include <stdexcept>
//File printing
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

// #include "llvm/IR/DataLayout.h" //to compute size of types?
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/DataLayout.h"


#include "llvm/Support/Signals.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/SystemUtils.h"


class CodeGen : c4::model::expression::IExpressionCodeGenVisitor, public c4::util::IASTVisitor {
    //"Global" stuff

    typedef std::unordered_map<std::string, c4::model::ctype::CTypedValue> Scope;
    
    class ScopeStack : std::vector<Scope> {
    public:
        ScopeStack() {
            pushScope();
        }

        void pushScope() {
            Scope s;
            push_back(s);
        }

        void popScope() {
            if(size() <= 1) {
                throw std::logic_error("Trying to pop the global scope!");
            }
            pop_back();
        }

        c4::model::ctype::CTypedValue operator[](const std::string& name) const {
            bool notFound = true;
            for(auto it=rbegin(); notFound && it<rend(); it++) {
                auto current = *it;
                if(current.count(name)) {
                    return (*current.find(name)).second;
                }
            }
            //Not found!
            throw std::logic_error("Name not present in the ScopeStack table");
            return c4::model::ctype::CTypedValue(nullptr, nullptr);
        }

        bool count(const std::string& name) const {
            for(auto it=rbegin(); it<rend(); it++) {
                auto current = *it;
                if(current.count(name)) {
                    return true;
                }
            }
            return false;
        }

        void set(const std::string& name, const c4::model::ctype::CTypedValue& val) {
            back().insert({name, val});
        }
    };

    std::string filename;
    ScopeStack scope; //Every time a new block is introduced, a new scope layer should be created, 
    llvm::LLVMContext ctx;
    llvm::Module M; //1:1 with translation units i.e. source file 
    llvm::IRBuilder<> builder, allocaBuilder;

    //Helper functions
    llvm::AllocaInst* Alloca(llvm::Type* type);
    llvm::Value* ptrToInt64(llvm::Value* value);
    llvm::Value* intToBool(llvm::Value* value, bool negated=false);
    void evaluateCondition(c4::model::ctype::CTypedValue& ctv, bool negated);
    void unifyIntegerSize(c4::model::ctype::CTypedValue &lhs, c4::model::ctype::CTypedValue &rhs, llvm::BasicBlock* insertLeftHere, llvm::BasicBlock* insertRightHere);
    void unifyIntegerSize(c4::model::ctype::CTypedValue &lhs, c4::model::ctype::CTypedValue &rhs); //Automatically uses current block for both
    void pointerAddInt(c4::model::ctype::CTypedValue &base, const c4::model::ctype::CTypedValue &index);
    llvm::Value* funcToPtr(llvm::Value* func);
    c4::model::ctype::CTypedValue loadFromLValue(const c4::model::expression::IExpression& expr);
    void convertToINT(c4::model::ctype::CTypedValue& integer); //Argument must be integer type




    //Visitor declarations
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::BinaryExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::CallExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::ConditionalExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::ConstantExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::IdentifierExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::IndexExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::MemberExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::SizeOfType &expr) override;
    virtual c4::model::ctype::CTypedValue visitLValue(const c4::model::expression::UnaryExpression &expr) override;

    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::BinaryExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::CallExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::ConditionalExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::ConstantExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::IdentifierExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::IndexExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::MemberExpression &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::SizeOfType &expr) override;
    virtual c4::model::ctype::CTypedValue visitRValue(const c4::model::expression::UnaryExpression &expr) override;
    
    void visit(const c4::model::statement::CompoundStatement& s)override;
    void visit(const c4::model::statement::ExpressionStatement& s)override;
    void visit(const c4::model::statement::IterationStatement& s)override;
    void visit(const c4::model::statement::JumpStatement& s)override;
    void visit(const c4::model::statement::LabeledStatement& s)override;
    void visit(const c4::model::statement::SelectionStatement& s)override;
    void visit(const c4::model::declaration::Declaration& s)override;
    void visit(const c4::model::declaration::DeclarationSpecifier & s)override;
    void visit(const c4::model::declaration::Declarator& s)override;
    void visit(const c4::model::declaration::DirectDeclarator& s)override;
    void visit(const c4::model::declaration::DirectDeclarator2& s)override;
    void visit(const c4::model::declaration::FunctionDefinition& s)override;
    void visit(const c4::model::declaration::ParameterDeclaration& s)override;
    void visit(const c4::model::declaration::ParameterTypeList& s)override;
    void visit(const c4::model::declaration::Pointer& s)override;
    void visit(const c4::model::declaration::Root & s)override;
    void visit(const c4::model::declaration::StructDeclarationList & s)override;
    void visit(const c4::model::declaration::StructUnionSpecifier & s)override; 
public:
    CodeGen(const std::string& filename) 
    : filename(filename), ctx(), M(filename, ctx), builder(ctx), allocaBuilder(ctx)
    {}

    int codeGenTest();
};