#pragma once

#include "CType.h"
#include "llvm/IR/Type.h"

namespace c4::model::ctype {

class CTypedValue {
public:
    Value* value;
    std::shared_ptr<const CType> type;

    CTypedValue() {}
    
    CTypedValue(Value* value, std::shared_ptr<const CType> type) : value(value), type(type)
    {}

    CTypedValue(const CTypedValue& ctv) = default;

    static CTypedValue invalid() {
        return CTypedValue(nullptr, nullptr);
    }

    Type* getLLVMType(IRBuilder<> &builder) const {
        return type->getLLVMType(builder);
    }

    bool isValid() {
        return value != nullptr && type != nullptr;
    }
    
};

}