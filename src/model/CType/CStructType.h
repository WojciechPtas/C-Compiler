#pragma once

#include "CType.h"
#include <unordered_map>
#include <vector>
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace c4::model::ctype {
    class CStructType : public CType {

    public:
        const std::string name;
        std::vector<std::string> fieldNames;
        std::vector<std::shared_ptr<const CType>> fieldTypes;

        CStructType(
            const std::string name,
            std::vector<std::string> fieldNames,
            std::vector<std::shared_ptr<const CType>> fieldTypes,
            int indirections
        ) 
        : CType(indirections, STRUCT), name(name), fieldNames(fieldNames), fieldTypes(fieldTypes) {}

        CStructType(
            const std::string name,
            std::vector<std::string> fieldNames,
            std::vector<std::shared_ptr<const CType>> fieldTypes
        ) 
        : CStructType(name, fieldNames, fieldTypes, 0) {}

        virtual std::shared_ptr<const CType> dereference() const override {
            return std::make_shared<CStructType>(name, fieldNames, fieldTypes, indirections-1);
        }

        virtual std::shared_ptr<const CType> addStar() const override {
            return std::make_shared<CStructType>(name, fieldNames, fieldTypes, indirections+1);
        }

        virtual bool compatible(const CType* another) const override;
        StructType* getLLVMStructType(IRBuilder<> &builder) const;
        virtual Type* getLLVMType(IRBuilder<> &builder) const override;
    };
}