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

StructType* CStructType::getLLVMStructType(llvm::LLVMContext &ctx) const {
    std::vector<Type*> fields;
    for(auto& type : fieldTypes) {
        fields.push_back(type->getLLVMType(ctx));
    }
    return StructType::create(ctx, fields);
}

Type* CStructType::getLLVMType(llvm::LLVMContext &ctx) const {
    if(indirections > 0) {
        return PointerType::getUnqual(ctx);
    }
    else {
        return getLLVMStructType(ctx);
    }
}