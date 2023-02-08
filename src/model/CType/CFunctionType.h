#pragma once

#include "CType.h"

using namespace llvm;

namespace c4::model::ctype {

    class CFunctionType : public CType {

    public:
        std::shared_ptr<const CType> retType;
        std::vector<std::shared_ptr<const CType>> paramTypes;
        
        CFunctionType(
            std::shared_ptr<const CType> retType,
            std::vector<std::shared_ptr<const CType>> &paramTypes,
            int indirections
        ) : CType(indirections, FUNCTION), retType(retType), paramTypes(paramTypes) {}

        CFunctionType(
            std::shared_ptr<const CType> retType,
            std::vector<std::shared_ptr<const CType>> &paramTypes
        ) : CFunctionType(retType, paramTypes, 0) {}

        virtual std::shared_ptr<const CType> dereference() const override {
            return nullptr;
            // return std::make_shared<CFunctionType>(retType, paramTypes, indirections-1);
        }
        
        virtual std::shared_ptr<const CType> addStar() const override {
            return nullptr;
            // return std::make_shared<CFunctionType>(retType, paramTypes, indirections+1);
        }

        virtual bool compatible(const CType* another) const override;

        virtual Type* getLLVMType(IRBuilder<> &builder) const override;

        FunctionType* getLLVMFuncType(IRBuilder<> &builder) const;
    };
 
}