#include "CStructType.h"

using namespace c4::model::ctype;

bool CStructType::compatible(const CType* another) const {
    if(this->kind != another->kind) {
        return false;
    }
    // else return "They have the same name"
    return (
        dynamic_cast<const CStructType*>(another)->name.compare(this->name)
    ) == 0;
}

StructType* CStructType::getLLVMStructType(IRBuilder<> &builder) const {
    std::vector<Type*> fields;
    for(auto& type : fieldTypes) {
        fields.push_back(type->getLLVMType(builder));
    }
    return StructType::create(builder.getContext(), fields, name);
}

Type* CStructType::getLLVMType(IRBuilder<> &builder) const {
    return getLLVMStructType(builder);
}