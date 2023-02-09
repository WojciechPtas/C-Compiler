#pragma once

#include "CType.h"

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

        bool isInteger() const override {
            return (t == TypeSpecifier::INT) || (t == TypeSpecifier::CHAR);
        }

        virtual bool compatible(const CType* another) const override;

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;
    };
}