#include "CStructType.h"

using namespace c4::model::ctype;
using namespace llvm;

bool CStructType::compatible(const CType* another) const {
    if(this->kind != another->kind || 
        this->indirections != another->indirections) {
        return false;
    }
    // else return "none of them is anonymous " and "They have the same name"
    auto casted = dynamic_cast<const CStructType*>(another);
    return (
        (!this->name.empty()) && (!casted->name.empty()) &&
        (casted->name.compare(this->name) == 0)
    );
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