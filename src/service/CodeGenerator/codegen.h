#pragma once

#include "../../model/ASTNode.h"
#include "../../model/expression/IExpressionCodeGenVisitor.h"
#include "../../model/CType/BaseCType.h"
#include "../../model/CType/CFunctionType.h"
#include "../../model/CType/ParameterInfo.h"
#include "../../model/CType/CTypedValue.h"

#include <unordered_map>
#include <iostream>

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

#include "llvm/Support/Signals.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/SystemUtils.h"

using namespace c4::model::ctype;
using namespace c4::model::expression;
using namespace llvm;

class CodeGen : IExpressionCodeGenVisitor {
    //"Global" stuff

    typedef std::unordered_map<std::string, CTypedValue> Scope;
    
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

        CTypedValue operator[](const std::string& name) const {
            bool notFound = true;
            for(auto it=rbegin(); notFound && it<rend(); it++) {
                auto current = *it;
                if(current.count(name)) {
                    return (*current.find(name)).second;
                }
            }
            //Not found!
            throw std::logic_error("Name not present in the ScopeStack table");
            return CTypedValue(nullptr, nullptr);
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

        void set(const std::string& name, const CTypedValue& val) {
            back().insert({name, val});
        }
    };

    std::string filename;
    ScopeStack scope; //Every time a new block is introduced, a new scope layer should be created, 
    LLVMContext ctx;
    Module M; //1:1 with translation units i.e. source file 
    IRBuilder<> builder, allocaBuilder;

    //Helper functions
    AllocaInst* Alloca(Type* type);
    void dereferenceAndLoad(CTypedValue &ctv);


    //Visitor declarations
    virtual CTypedValue visitLValue(const BinaryExpression &expr) override;
    virtual CTypedValue visitLValue(const CallExpression &expr) override;
    virtual CTypedValue visitLValue(const ConditionalExpression &expr) override;
    virtual CTypedValue visitLValue(const ConstantExpression &expr) override;
    virtual CTypedValue visitLValue(const IdentifierExpression &expr) override;
    virtual CTypedValue visitLValue(const IndexExpression &expr) override;
    virtual CTypedValue visitLValue(const MemberExpression &expr) override;
    virtual CTypedValue visitLValue(const SizeOfType &expr) override;
    virtual CTypedValue visitLValue(const UnaryExpression &expr) override;

    virtual CTypedValue visitRValue(const BinaryExpression &expr) override;
    virtual CTypedValue visitRValue(const CallExpression &expr) override;
    virtual CTypedValue visitRValue(const ConditionalExpression &expr) override;
    virtual CTypedValue visitRValue(const ConstantExpression &expr) override;
    virtual CTypedValue visitRValue(const IdentifierExpression &expr) override;
    virtual CTypedValue visitRValue(const IndexExpression &expr) override;
    virtual CTypedValue visitRValue(const MemberExpression &expr) override;
    virtual CTypedValue visitRValue(const SizeOfType &expr) override;
    virtual CTypedValue visitRValue(const UnaryExpression &expr) override;

public:
    CodeGen(const std::string& filename) 
    : filename(filename), M(filename, ctx), builder(ctx), allocaBuilder(ctx)
    {}

    int codeGenTest();
};