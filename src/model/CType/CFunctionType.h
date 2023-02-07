#pragma once

#include "CType.h"

using namespace llvm;

namespace c4::model::ctype {

    class CFunctionType : public CType {
        std::shared_ptr<CType> retType;
        std::vector<std::shared_ptr<CType>> paramTypes;

    public:
        CFunctionType(
            std::shared_ptr<CType> retType,
            std::vector<std::shared_ptr<CType>> paramTypes,
            uint indirections
        ) : CType(indirections, FUNCTION), retType(retType), paramTypes(paramTypes) {}

        CFunctionType(
            std::shared_ptr<CType> retType,
            std::vector<std::shared_ptr<CType>> paramTypes
        ) : CType(0, FUNCTION), retType(retType), paramTypes(paramTypes) {}

        virtual bool equals(const CType* another) const override;
        

        virtual Type* getLLVMType(IRBuilder<> &builder) const override;

        FunctionType* getLLVMFuncType(IRBuilder<> &builder) const;
    };
 
}