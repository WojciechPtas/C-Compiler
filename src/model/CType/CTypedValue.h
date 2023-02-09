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

    bool isConst() {
        //Func with 1 indirection represent function designators, with a modifiable lvalue
        //Simple functions are not modifiable
        //If necessary, refactor in the future to have setter that throws an exception
        return type->isFuncNonDesignator();
    }

    //Calls dereference on its type, decrementing the indirections counter
    //Beware! It will not check for a negative number
    void dereference() {
        type = type->dereference();
    }

    bool compatible(const CTypedValue& another) {
        return this->type->compatible(another.type.get());
    }
    
};

}