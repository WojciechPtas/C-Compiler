#pragma once

#include <vector>
#include "CType.h"
#include "llvm/IR/DerivedTypes.h"

namespace c4::model::ctype {
    class CStructType : public CType {
        std::unordered_map<std::string, uint> memberIndexes;

    public:
        const std::string name;
        const std::vector<std::string> fieldNames;
        const std::vector<std::shared_ptr<const CType>> fieldTypes;

        CStructType(
            const std::string &name,
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes,
            int indirections
        ) 
        : CType(indirections, STRUCT), name(name), fieldNames(fieldNames), fieldTypes(fieldTypes) {
            for(uint i=0; i<fieldNames.size(); i++) {
                memberIndexes[fieldNames[i]] = i;
            }
        }

        CStructType(
            const std::string &name,
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes
        ) 
        : CStructType(name, fieldNames, fieldTypes, 0) {}

        virtual std::shared_ptr<const CType> dereference() const override {
            return std::make_shared<CStructType>(name, fieldNames, fieldTypes, indirections-1);
        }

        virtual std::shared_ptr<const CType> addStar() const override {
            return std::make_shared<CStructType>(name, fieldNames, fieldTypes, indirections+1);
        }

        bool isInteger() const override {
            return false;
        }

        bool isBool() const override {
            return false;
        }

        bool hasMember(const std::string& fieldName) const {
            return memberIndexes.count(fieldName);
        }
        
        uint getIndexOf(const std::string& fieldName) const {
            return memberIndexes.find(name)->second;
        }

        virtual bool compatible(const CType* another) const override;
        llvm::StructType* getLLVMStructType(llvm::LLVMContext &ctx) const; //THIS WILL NOT DISCRIMINATE BETWEEN IF POINTER OR NOT, DO NECESSARY CHECKS BEFORE USING THIS
        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;
        
    };
}