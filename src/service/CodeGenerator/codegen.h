#pragma once

#include "../../model/ASTNode.h"
#include "../../model/expression/IExpressionCodeGenVisitor.h"
#include "../../model/expression/IExpression.h"
#include "../../model/CType/BaseCType.h"
#include "../../model/CType/CFunctionType.h"
#include "../../model/CType/CStructType.h"
#include "../../model/CType/ConstantZero.h"
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
#include "../../model/declaration/TypeName.h"

#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <stack>
#include <unordered_set>
//File printing
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"


#include "../../util/token/PrintVisitor.h"
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
    enum class ErrorState { //For now pretty simple. Add states as desired, you'll associate a string to every one of them later.
        OK=0,
        ERROR
    };
    struct Var{
    std::string name;
    std::string structname="";
    std::shared_ptr<c4::model::ctype::CType> type;
    std::shared_ptr<c4::model::ctype::ParametersInfo> params;
};
    struct Scope {
        std::unordered_map<std::string, c4::model::ctype::CTypedValue> variableDeclars;
        std::unordered_map<std::string, std::shared_ptr<c4::model::ctype::CStructType>> structDeclars;
    };
    class ScopeStack  {
        std::vector<Scope> stack;
        llvm::LLVMContext* ctx;
    public:
        ScopeStack(llvm::LLVMContext* ctx) : ctx(ctx) {
            pushScope();
        }
        // ScopeStack() : ScopeStack(nullptr) {}


        void pushScope() {
            Scope s;
            stack.push_back(s);
        }

        void popScope() {
            if(stack.size() <= 1) {
                throw std::logic_error("Trying to pop the global scope!");
            }
            stack.pop_back();
        }

        bool isGlobal() { //Returns true if the current scope is global
            return stack.size() == 1;
        }

        c4::model::ctype::CTypedValue operator[](const std::string& name) const {
            for(auto it=stack.rbegin(); it<stack.rend(); it++) {
                auto& currentVarDeclars = it->variableDeclars;
                if(currentVarDeclars.count(name)) {
                    return (*currentVarDeclars.find(name)).second;
                }
            }
            //Not found!
            throw std::logic_error("Name not present in the ScopeStack table. Check with count() before using the [] operator!");
            return c4::model::ctype::CTypedValue::invalid();
        }

        std::shared_ptr<const c4::model::ctype::CStructType> getStruct(const std::string& name) const {
            for(auto it=stack.rbegin(); it<stack.rend(); it++) {
                auto& currentStructDeclars = it->structDeclars;
                if(currentStructDeclars.count(name)) {
                    auto &retvalue = (*currentStructDeclars.find(name)).second;
                    return retvalue;
                }
            }
            //Not found!
            throw std::logic_error("Name not present in the ScopeStack table. Check with count() before using the [] operator!");
            return nullptr;
        }

        //Checks in all scopes
        bool isVarDeclared(const std::string& name) const {
            for(auto it=stack.rbegin(); it<stack.rend(); it++) {
                auto& current = it->variableDeclars;
                if(current.count(name)) {
                    return true;
                }
            }
            return false;
        }

        //Checks in all scopes
        bool isStructDeclared(const std::string& name) const {
            for(auto it=stack.rbegin(); it<stack.rend(); it++) {
                auto& current = it->structDeclars;
                if(current.count(name)) {
                    return true;
                }
            }
            return false;
        }

        //Checks in all scopes
        bool isStructDefined(const std::string& name) const {
            for(auto it=stack.rbegin(); it<stack.rend(); it++) {
                auto& current = it->structDeclars;
                if(current.count(name)) {
                    return current.find(name)->second->isDefined();
                }
            }
            return false;
        }

        //Only checks in current scope
        bool varAlreadyDeclared(const std::string& name) const {
            auto& topVarDeclars = stack.back().variableDeclars;
            return topVarDeclars.count(name);
        }

        //Only checks in current scope
        bool structAlreadyDeclared(const std::string& name) const {
            auto& topStructDeclars = stack.back().structDeclars;
            return topStructDeclars.count(name);
        }

        //Only checks in current scope
        bool structAlreadyDefined(const std::string& name) const {
            auto& topStructDeclars = stack.back().structDeclars;
            return topStructDeclars.count(name) && 
                topStructDeclars.find(name)->second->isDefined();
        }

        void declareVar(const std::string& name, const c4::model::ctype::CTypedValue& val) {
            if(varAlreadyDeclared(name)) {
                throw std::logic_error("Already declared in the same scope! Check this beforehand using varAlreadyDeclared()");
            }

            auto& topVarDeclars = stack.back().variableDeclars;
            topVarDeclars[name] = val;
        }

        //Does nothing if struct was already declared/defined in this current scope
        void declareStruct(const std::string& name) {
            if(!structAlreadyDeclared(name)) {
                auto& topStructDeclars = stack.back().structDeclars;
                topStructDeclars[name] = c4::model::ctype::CStructType::undefined();
            }
        }

        //Check structAlreadyDefined() before using this one
        void defineStruct(const std::string& name, std::shared_ptr<c4::model::ctype::CStructType> val) {
            if(structAlreadyDefined(name)) {
                throw std::logic_error("Struct already defined in the same scope! Check this beforehand using structAlreadyDefined()");
            }
            auto& topStructDeclars = stack.back().structDeclars;
            if(structAlreadyDeclared(name)) {
                topStructDeclars[name]->define(val);
            }
            else {
                topStructDeclars[name] = val;
            }
            topStructDeclars[name]->getLLVMStructType(*ctx); //This ensures the llvmtype gets cached because called on a non-const instance
        } 
    };

    std::string filename;
    llvm::LLVMContext ctx;
    ScopeStack scope; //Every time a new block is introduced, a new scope layer should be created, 

    llvm::Module M; //1:1 with translation units i.e. source file 
    llvm::IRBuilder<> builder, allocaBuilder;
    ErrorState state;
    std::unordered_map<std::string, llvm::BasicBlock*> gotoLabels; //constructed in the first phase
    std::stack<llvm::BasicBlock*> headerBlocks;
    std::stack<llvm::BasicBlock*> afterBlocks;
    bool FirstPhase;
    bool SecondPhase;
    bool ScopeDeclared;
    std::shared_ptr<const c4::model::ctype::CFunctionType> currentFunc;


    void setError(CodeGen::ErrorState state) {
        this->state = state;
        //insert big switch that prints the error here
    }

    //Helper functions

    llvm::AllocaInst* Alloca(llvm::Type* type, const std::string& name);
    llvm::AllocaInst* Alloca(const c4::model::ctype::CType* type, const std::string& name);
    llvm::AllocaInst* Alloca(std::shared_ptr<const c4::model::ctype::CType> type, const std::string& name);
    llvm::GlobalVariable* GlobalAlloca(llvm::Type* type, const std::string& name);
    llvm::GlobalVariable* GlobalAlloca(const c4::model::ctype::CType* type, const std::string& name);
    llvm::GlobalVariable* GlobalAlloca(std::shared_ptr<const c4::model::ctype::CType> type, const std::string& name);




    void evaluateCondition(c4::model::ctype::CTypedValue& ctv, bool negated);

    llvm::Value* ptrToInt64(llvm::Value* value);
    llvm::Value* intToBool(llvm::Value* value, bool negated=false);
    void unifyIntegerSize(c4::model::ctype::CTypedValue &lhs, c4::model::ctype::CTypedValue &rhs, llvm::BasicBlock* insertLeftHere, llvm::BasicBlock* insertRightHere);
    void unifyIntegerSize(c4::model::ctype::CTypedValue &lhs, c4::model::ctype::CTypedValue &rhs); //Automatically uses current block for both
    void pointerAddInt(c4::model::ctype::CTypedValue &base, const c4::model::ctype::CTypedValue &index);
    llvm::Value* funcToPtr(llvm::Value* func);
    c4::model::ctype::CTypedValue loadFromLValue(const c4::model::expression::IExpression& expr);
    void convertToINT(c4::model::ctype::CTypedValue& integer); //Argument must be integer type
    void constantZeroToInt(c4::model::ctype::CTypedValue& v1);
    void constantZeroToVoidPtr(c4::model::ctype::CTypedValue& v1);
    void matchConstantZeroLeft(c4::model::ctype::CTypedValue& zero, const c4::model::ctype::CType* v2);
    void matchConstantZero(c4::model::ctype::CTypedValue& v1, c4::model::ctype::CTypedValue& v2);


    void reportError(std::shared_ptr<const c4::model::token::Token> token, std::string errorMessage){
        c4::util::token::PrintVisitor v(std::cerr);
        v.printPosition(*token);
        std::cerr<<"error: " <<errorMessage<<"\n";
        setError(CodeGen::ErrorState::ERROR);
    }
    
    std::shared_ptr<const c4::model::ctype::CType> getCtype(const std::shared_ptr<const c4::model::declaration::TypeName>&s);
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
    void visit(const c4::model::declaration::TypeName & s) override;



    Var buildParam(std::shared_ptr<c4::model::declaration::ParameterDeclaration> param);
    c4::model::ctype::ParametersInfo buildParameters(std::shared_ptr<c4::model::declaration::ParameterTypeList> params);
    Var buildVar(std::shared_ptr<c4::model::declaration::DirectDeclarator> p, Var returnVal);
    Var buildVar(std::shared_ptr<c4::model::declaration::Pointer> p, Var returnVal);
    Var buildVar(std::shared_ptr<c4::model::declaration::Declarator> d, Var returnVal);
    Var buildFromDS(std::shared_ptr<c4::model::declaration::DeclarationSpecifier> ds);
    Var buildDeclarationFromDS(std::shared_ptr<c4::model::declaration::DeclarationSpecifier> ds);
    Var buildStruct(std::shared_ptr<c4::model::declaration::StructUnionSpecifier> s);
    c4::model::ctype::ParametersInfo buildStruct(std::shared_ptr<c4::model::declaration::StructDeclarationList> s);
    Var buildfromDeclaration(std::shared_ptr<c4::model::declaration::Declaration> d);

public:
    CodeGen(const std::string& filename) 
    : filename(filename), ctx(), scope(&ctx), M(filename, ctx), builder(ctx), allocaBuilder(ctx), state(ErrorState::OK), FirstPhase(false), SecondPhase(false), ScopeDeclared(false)
    {
        size_t extensionDotPos = this->filename.rfind('.');
        if(extensionDotPos == std::string::npos) { //Not found 
            this->filename.append(".ll");
        }
        else {
            this->filename.erase(extensionDotPos); //From that position onwards
            this->filename.append(".ll");
        }
    }
    
    bool isError() {
        return state != CodeGen::ErrorState::OK;
    }
    int codeGenTest();
    void printIR(bool debug);

};