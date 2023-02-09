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

    //Use this in case of error
    static CTypedValue invalid() {
        return CTypedValue(nullptr, nullptr);
    }

    llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const {
        return type->getLLVMType(ctx);
    }

    bool isValid() {
        return value != nullptr && type != nullptr;
    }

    //Calls dereference on its type, decrementing the indirections counter
    //Beware! It will not check for a negative number
    void dereference() {
        type = type->dereference();
    }
    
};

}