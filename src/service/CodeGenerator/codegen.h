#pragma once

#include "../../model/ASTNode.h"

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

using namespace c4;
using namespace llvm;

class CodeGen {
    //"Global" stuff

    enum TypeSpecifier {
        VOID,
        INT,
        CHAR
    };

    class CType {
    protected:
        uint indirections;

    public:
        CType(uint indirections) : indirections(indirections) {}
        virtual Type* getLLVMType(IRBuilder<> &builder) const = 0;
        void incIndirections() {
            indirections++;
        }
        void decIndirections() {
            indirections--;
        }
    };

    //Perhaps implement BaseCType::get() to be able to do pointer equality
    class BaseCType : public CType {
    private:
        TypeSpecifier t;

    public:
        BaseCType(TypeSpecifier t, uint indirections) 
        : CType(indirections), t(t) 
        {}

        BaseCType(TypeSpecifier t) 
        : CType(0), t(t) 
        {}

        virtual Type* getLLVMType(IRBuilder<> &builder) const override {
            if(indirections) {
                return builder.getPtrTy();
            }
            switch(t) {
                case INT:
                    return builder.getInt32Ty();
                case CHAR:
                    return builder.getInt8Ty();
                case VOID: 
                    return builder.getVoidTy();
                default:
                    throw std::logic_error("Unrecognized type");
            }
            return NULL;
        }
    };

    class CFunctionType : public CType {
        CType* retType;
        std::vector<CType*> paramTypes;

    public:
        CFunctionType(
            CType* retType,
            std::vector<CType*> paramTypes,
            uint indirections
        ) : CType(indirections), retType(retType), paramTypes(paramTypes) {}

        CFunctionType(
            CType* retType,
            std::vector<CType*> paramTypes
        ) : CType(0), retType(retType), paramTypes(paramTypes) {}
        

        FunctionType* getLLVMFuncType(IRBuilder<> &builder) const {
            Type* llvmRetType = retType->getLLVMType(builder);
            std::vector<Type*> llvmParamTypes;
            for(CType* ctype : paramTypes) {
                llvmParamTypes.push_back(ctype->getLLVMType(builder));
            }
            return FunctionType::get(llvmRetType, llvmParamTypes, false);
        }

        virtual Type* getLLVMType(IRBuilder<> &builder) const override {
            return getLLVMFuncType(builder);
        }
    };
        
    class CTypedLValue {
        Value* lvalue;
        CType* type;

    public:
        CTypedLValue(Value* lvalue, CType* type) : lvalue(lvalue), type(type) {}

        Type* getLLVMType(IRBuilder<> &builder) {
            return type->getLLVMType(builder);
        }

        Value* getValue() {
            return lvalue;
        }
        
    };

    class ParametersInfo {
    public:
        std::vector<std::string> names;
        std::vector<CType*> types;
        
        std::vector<Type*> getLLVMTypes(IRBuilder<> &builder) {
            std::vector<Type*> ret;
            for(CType* ctype : types) {
                ret.push_back(ctype->getLLVMType(builder));
            }
            return ret;
        }
    };

    class Scope {
        std::unordered_map<std::string, CTypedLValue*> current;
        Scope* outer = NULL;

    public:
        Scope() = default;
        
        Scope(Scope* outer) : outer(outer) 
        {}

        CTypedLValue* operator[](const std::string& name) {
            if(current.count(name)) {
                return current[name];
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

        void set(const std::string& name, CTypedLValue* val) {
            current[name] = val;
        }
    };

    std::string filename;
    std::unordered_map<std::string, Function*> functions;
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