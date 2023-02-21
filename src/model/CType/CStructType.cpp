#include "CStructType.h"

using namespace c4::model::ctype;
using namespace llvm;

bool CStructType::compatible(const CType* another) const {
    if(this == another) { //Same address!
         return true;
    }
    else if(this->kind != another->kind || 
        this->indirections != another->indirections) {
        return false;
    }
    // else return "they have the same originalStruct address"
    auto casted = dynamic_cast<const CStructType*>(another);
    return (this->originalStruct == casted->originalStruct);
}

bool CStructType::assignmentCompatible(const CType* another) const {
    return this->compatible(another) ||
        (this->isPointer() && another->isVoidStar());
}

StructType* CStructType::getLLVMStructType(llvm::LLVMContext &ctx) const {
    if(originalStruct->cachedType) {
        return originalStruct->cachedType;
    }
    else if(!isOriginal()) {
        return originalStruct->getLLVMStructType(ctx);
    }
    //else
    std::vector<Type*> fields;
    for(auto& type : fieldTypes) {
        fields.push_back(type->getLLVMType(ctx));
    }
    StructType* retval = StructType::create(ctx, fields);
    retval->setName(this->getName());
    return retval;
}

StructType* CStructType::getLLVMStructType(llvm::LLVMContext &ctx) {
    return originalStruct->cachedType = const_cast<const CStructType*>(this)->getLLVMStructType(ctx);
}

Type* CStructType::getLLVMType(llvm::LLVMContext &ctx) const {
    if(indirections > 0) {
        return PointerType::getUnqual(ctx);
    }
    else {
        return getLLVMStructType(ctx);
    }
}

Type* CStructType::getLLVMType(llvm::LLVMContext &ctx) {//Updates cache
    if(indirections > 0) {
        return PointerType::getUnqual(ctx);
    }
    else {
        return getLLVMStructType(ctx);
    }
}