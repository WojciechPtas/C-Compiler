#pragma once

#include "CType.h"
#include "llvm/IR/DerivedTypes.h"

namespace c4::model::ctype {

    class CFunctionType : public CType {

    public:
        const std::shared_ptr<const CType> retType;
        const std::vector<std::shared_ptr<const CType>> paramTypes;
        
        CFunctionType(
            std::shared_ptr<const CType> retType,
            const std::vector<std::shared_ptr<const CType>> &paramTypes,
            int indirections
        ) : CType(indirections, FUNCTION), retType(retType), paramTypes(paramTypes) {}

        CFunctionType(
            std::shared_ptr<const CType> retType,
            const std::vector<std::shared_ptr<const CType>> &paramTypes
        ) : CFunctionType(retType, paramTypes, 0) {}

        virtual std::shared_ptr<const CType> dereference() const override {
            // return nullptr;
            return std::make_shared<CFunctionType>(retType, paramTypes, indirections-1);
        }
        
        virtual std::shared_ptr<const CType> addStar() const override {
            // return nullptr;
            return std::make_shared<CFunctionType>(retType, paramTypes, indirections+1);
        }
        
        bool isInteger() const override {
            return false;
        }

        bool isBool() const override {
            return false;
        }

        virtual bool compatible(const CType* another) const override;

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;

        llvm::FunctionType* getLLVMFuncType(llvm::LLVMContext &ctx) const;
    };
 
}