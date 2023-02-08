#include "CStructType.h"

using namespace c4::model::ctype;
using namespace llvm;

bool CStructType::compatible(const CType* another) const {
    if(this->kind != another->kind) {
        return false;
    }
    // else return "They have the same name"
    return (
        dynamic_cast<const CStructType*>(another)->name.compare(this->name)
    ) == 0;
}

StructType* CStructType::getLLVMStructType(llvm::LLVMContext &ctx) const {
    std::vector<Type*> fields;
    for(auto& type : fieldTypes) {
        fields.push_back(type->getLLVMType(ctx));
    }
    return StructType::create(ctx, fields, name);
}

Type* CStructType::getLLVMType(llvm::LLVMContext &ctx) const {
    return getLLVMStructType(ctx);
}