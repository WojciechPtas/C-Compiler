#pragma once

#include "CType.h"

namespace c4::model::ctype {
    enum TypeSpecifier {
        VOID,
        INT,
        CHAR,
        BOOL
    };

    class BaseCType : public CType {
    public:
        const TypeSpecifier t;

        //If you need a shared_ptr, consider using BaseCType::get() instead
        BaseCType(TypeSpecifier t, int indirections) 
        : CType(indirections, BASE), t(t) 
        {}

        //If you need a shared_ptr, consider using BaseCType::get() instead
        BaseCType(TypeSpecifier t) 
        : BaseCType(t, 0) 
        {}

        virtual std::shared_ptr<const CType> dereference() const override {
            return BaseCType::get(t, indirections-1);
        }
        
        virtual std::shared_ptr<const CType> addStar() const override {
            return BaseCType::get(t, indirections+1);
        }

        static std::shared_ptr<const BaseCType> get(TypeSpecifier ts, int indirections) {
            return std::make_shared<const BaseCType>(ts, indirections);
        }
        static std::shared_ptr<const BaseCType> get(TypeSpecifier ts) {
            return BaseCType::get(ts, 0);
        }

        bool isInteger() const override {
            return (t == TypeSpecifier::INT) || (t == TypeSpecifier::CHAR) || (t == TypeSpecifier::BOOL);
        }

        bool isBool() const override {
            return t == TypeSpecifier::BOOL;
        }

        virtual bool compatible(const CType* another) const override;
        virtual bool equivalent(const CType* another) const { //As compatible, but requires the integer size to be the same
            if(this->compatible(another)) {
                auto casted = dynamic_cast<const BaseCType*>(another);
                return this->t == casted->t;
            }
            else {
                return false;
            }
        }

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;
    };
}