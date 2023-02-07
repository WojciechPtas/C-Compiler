#pragma once

#include "CType.h"

using namespace llvm;

namespace c4::model::ctype {
    //Perhaps implement BaseCType::get() to be able to do pointer equality
    class BaseCType : public CType {
    private:
        TypeSpecifier t;

    public:
        BaseCType(TypeSpecifier t, uint indirections) 
        : CType(indirections, NOT_A_FUNCTION), t(t) 
        {}

        BaseCType(TypeSpecifier t) 
        : CType(0, NOT_A_FUNCTION), t(t) 
        {}

        virtual bool equals(const CType* another) const override;

        virtual Type* getLLVMType(IRBuilder<> &builder) const override;
    };
}