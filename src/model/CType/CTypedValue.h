#pragma once

#include "CType.h"

namespace c4::model::ctype {

class CTypedValue {
public:
    llvm::Value* value;
    std::shared_ptr<const CType> type;

    CTypedValue() {}
    
    CTypedValue(llvm::Value* value, std::shared_ptr<const CType> type) : value(value), type(type)
    {}

    CTypedValue(const CTypedValue& ctv) = default;

    static CTypedValue invalid() {
        return CTypedValue(nullptr, nullptr);
    }

    llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const {
        return type->getLLVMType(ctx);
    }

    bool isValid() {
        return value != nullptr && type != nullptr;
    }
    
};

}