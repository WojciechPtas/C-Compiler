#pragma once

#include "../../model/ASTNode.h"
#include "../../model/expression/IExpressionCodeGenVisitor.h"
#include "../../model/CType/BaseCType.h"
#include "../../model/CType/CFunctionType.h"
#include "../../model/CType/ParameterInfo.h"

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
using namespace llvm;

class CodeGen {
    //"Global" stuff

        
    class CTypedLValue {
        Value* lvalue;
        std::shared_ptr<CType> type;

    public:
        CTypedLValue(Value* lvalue, const std::shared_ptr<CType>& type) : lvalue(lvalue), type(type) {}

        Type* getLLVMType(IRBuilder<> &builder) const {
            return type->getLLVMType(builder);
        }

        Value* getValue() const {
            return lvalue;
        }
        
    };


    class Scope {
        std::unordered_map<std::string, std::shared_ptr<CTypedLValue>> current;
        Scope* outer = NULL;

    public:
        Scope() = default;
        
        Scope(Scope* outer) : outer(outer) 
        {}

        CTypedLValue const* operator[](const std::string& name) const {
            if(current.count(name)) {
                return (*current.find(name)).second.get();
            }
            else if(outer == NULL) {
                return (*outer)[name];
            }
            else {
                throw std::logic_error("Name not present in the Scope table");
            }
            return NULL;
        }

        bool count(const std::string& name) const {
            return current.count(name) || 
                ((outer != NULL) ? outer->count(name) : false);
        }

        //Parameter will be copied in a shared_ptr
        void set(const std::string& name, const CTypedLValue& val) {
            current[name] = std::make_shared<CTypedLValue>(val);
        }
    };

    std::string filename;
    Scope global;
    LLVMContext ctx;
    Module M; //1:1 with translation units i.e. source file 
    IRBuilder<> builder, allocaBuilder;

    AllocaInst* Alloca(Type* type);

public:
    CodeGen(const std::string& filename) 
    : filename(filename), M(filename, ctx), builder(ctx), allocaBuilder(ctx)
    {}

    int codeGenTest();
};