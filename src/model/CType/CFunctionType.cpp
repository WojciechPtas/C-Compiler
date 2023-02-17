#include "CFunctionType.h"

using namespace c4::model::ctype;
using namespace llvm;

bool CFunctionType::compatible(const CType* another) const {
    if(this == another) { //Same address!
         return true;
    }
    else if(
        this->kind != another->kind ||
        (this->indirections != another->indirections &&
            !(this->isFunc() && another->isFunc())
        )
    ) {
        return false;
    }
    //else
    const CFunctionType* casted = dynamic_cast<const CFunctionType*>(another);
    //Ret type and parameters must be equivalent!
    bool condition = 
        (this->paramTypes.size() == casted->paramTypes.size()) &&
        (this->retType->equivalent(casted->retType.get()));

    for(int i=paramTypes.size()-1; condition && i>=0; i--) {
        condition = condition && 
            this->paramTypes[i]->equivalent(casted->paramTypes[i].get());
    }
    return condition;
}

bool CFunctionType::equivalent(const CType* another) const {
    return this->indirections == another->indirections && 
        this->compatible(another);
}


// //Returns true if the two functions have conflicting types, i.e. no declaration of both under the same name can coexist
// bool CFunctionType::conflicting(const CFunctionType* another) const {
//     if(this->compatible(another)) {
//         return false;
//     }
    

// }

Type* CFunctionType::getLLVMType(llvm::LLVMContext &ctx) const {
    // if(indirections <= 0) {
    //     throw std::logic_error("Function designators cannot have 0 indirections");
    // }
    // Function and pointer to function are gonna be the same thing
    if (indirections > 1) { //pointer to pointer...
        return PointerType::getUnqual(ctx);
    }
    //else
    return getLLVMFuncType(ctx);
}

FunctionType* CFunctionType::getLLVMFuncType(llvm::LLVMContext &ctx) const {
    if (indirections > 0) {
        std::logic_error("Called getLLVMFuncType on a function pointer. Use getLLVMType instead");
    }
    Type* llvmRetType = retType->getLLVMType(ctx);
    std::vector<Type*> llvmParamTypes;
    for(auto& ctype : paramTypes) {
        llvmParamTypes.push_back(ctype->getLLVMType(ctx));
    }
    return FunctionType::get(llvmRetType, llvmParamTypes, false);
}