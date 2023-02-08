#pragma once

#include "CType.h"

using namespace llvm;

namespace c4::model::ctype {
    class BaseCType : public CType {
    public:
        const TypeSpecifier t;

        BaseCType(TypeSpecifier t, int indirections) 
        : CType(indirections, BASE), t(t) 
        {}

        BaseCType(TypeSpecifier t) 
        : BaseCType(t, 0) 
        {}

        virtual std::shared_ptr<const CType> dereference() const override {
            return std::make_shared<BaseCType>(t, indirections-1);
        }
        
        virtual std::shared_ptr<const CType> addStar() const override {
            return std::make_shared<BaseCType>(t, indirections+1);
        }

        virtual bool compatible(const CType* another) const override;

        virtual Type* getLLVMType(IRBuilder<> &builder) const override;
    };
}