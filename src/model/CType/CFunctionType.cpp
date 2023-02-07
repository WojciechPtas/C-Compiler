#include "CFunctionType.h"

using namespace c4::model::ctype;

bool CFunctionType::equals(const CType* another) const {
    if(!another->isFunction) {
        return false;
    }
    //else
    const CFunctionType* casted = dynamic_cast<const CFunctionType*>(another);
    
    bool condition = (this->paramTypes.size() == casted->paramTypes.size()) &&
        (this->retType->equals(casted->retType.get()));

    for(int i=paramTypes.size(); condition && i>0; i--) {
        condition = condition && 
            this->paramTypes[i]->equals(casted->paramTypes[i].get());
    }
    return condition;
}

Type* CFunctionType::getLLVMType(IRBuilder<> &builder) const {
    // if(indirections <= 0) {
    //     throw std::logic_error("Function designators cannot have 0 indirections");
    // }
    // Function and pointer to function are gonna be the same thing
    if (indirections > 1) { //pointer to pointer...
        return builder.getPtrTy();
    }
    //else
    return getLLVMFuncType(builder);
}

FunctionType* CFunctionType::getLLVMFuncType(IRBuilder<> &builder) const {
    if (indirections > 0) {
        std::logic_error("Called getLLVMFuncType on a function pointer. Use getLLVMType instead");
    }
    Type* llvmRetType = retType->getLLVMType(builder);
    std::vector<Type*> llvmParamTypes;
    for(std::shared_ptr<CType> ctype : paramTypes) {
        llvmParamTypes.push_back(ctype->getLLVMType(builder));
    }
    return FunctionType::get(llvmRetType, llvmParamTypes, false);
}