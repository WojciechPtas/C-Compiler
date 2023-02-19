#pragma once

#include <vector>
#include "CType.h"
#include "llvm/IR/DerivedTypes.h"

namespace c4::model::ctype {
    class CStructType : public CType {
        std::unordered_map<std::string, uint> memberIndexes;
        CStructType* originalStruct;
        llvm::StructType* cachedType = nullptr;
        bool defined;
        std::vector<std::string> fieldNames;
        std::vector<std::shared_ptr<const CType>> fieldTypes;
        std::string name;

    public:
        //For undeclared CStructTypes
        CStructType(int indirections) : CType(indirections, STRUCT), originalStruct(this), defined(false) {}
        CStructType() : CStructType(0) {}

        CStructType(
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes,
            int indirections,
            const std::string& name
        ) 
        :  CStructType(indirections) {
            define(fieldNames, fieldTypes, name);
        }

        CStructType(
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes,
            const std::string& name
        ) 
        : CStructType(fieldNames, fieldTypes, 0, name) {}

        CStructType(const CStructType& other, int indirections) : CType(indirections, STRUCT), originalStruct(other.originalStruct) {}
        CStructType(const CStructType& other) : CType(other.indirections, STRUCT), originalStruct(other.originalStruct) {}

        const std::vector<std::string>& getFieldNames() const {
            return originalStruct->fieldNames;
        }

        const std::vector<std::shared_ptr<const CType>>& getFieldTypes() const {
            return originalStruct->fieldTypes;
        }
        
        const std::string& getName() const {
            return originalStruct->name;
        }
        
        static std::shared_ptr<CStructType> get(
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes,
            const std::string& name
        ) {
            return std::make_shared<CStructType>(fieldNames, fieldTypes, name);
        }

        virtual std::shared_ptr<CType> dereference() const override {
            return std::make_shared<CStructType>(*this, indirections-1);
        }

        virtual std::shared_ptr<CType> addStar() const override {
            return std::make_shared<CStructType>(*this, indirections+1);
        }

        static std::shared_ptr<CStructType> undefined() {
            return std::make_shared<CStructType>();
        }

        void define(
            const std::vector<std::string> &fieldNames,
            const std::vector<std::shared_ptr<const CType>> &fieldTypes,
            const std::string& name
        ) {
            if(!this->isOriginal()) {
                throw std::logic_error("Defining non-original struct!");
            }
            if(this->isDefined()) { //Already
                throw std::logic_error("Redefining struct!");
            }
            this->defined = true;
            this->fieldNames = fieldNames;
            this->fieldTypes = fieldTypes;
            this->name = name;
            for(uint i=0; i<fieldNames.size(); i++) {
                this->memberIndexes[fieldNames[i]] = i;
            }
        }

        void define(std::shared_ptr<const CStructType> from) {
            if(!from->isDefined()) {
                throw std::logic_error("Defining from undeclared struct!");
            }
            if(!from->isOriginal()) {
                throw std::logic_error("Defining from non-original struct!");
            }

            define(from->fieldNames, from->fieldTypes, from->getName());
        }

        bool isInteger() const override {
            return false;
        }

        bool isBool() const override {
            return false;
        }

        bool isVoid() const override {
            return false;
        }

        bool isComplete() const override {
            return isPointer() || isDefined();
        }

        bool isDefined() const {
            return originalStruct->defined;
        }

        bool isOriginal() const {
            return this == originalStruct;
        }

        bool hasMember(const std::string& fieldName) const {
            return originalStruct->memberIndexes.count(fieldName);
        }
        
        uint getIndexOf(const std::string& fieldName) const {
            return originalStruct->memberIndexes.find(fieldName)->second;
        }

        virtual bool compatible(const CType* another) const override;
        llvm::StructType* getLLVMStructType(llvm::LLVMContext &ctx) const; //THIS WILL NOT DISCRIMINATE BETWEEN IF POINTER OR NOT, DO NECESSARY CHECKS BEFORE USING THIS
        llvm::StructType* getLLVMStructType(llvm::LLVMContext &ctx); //THIS WILL NOT DISCRIMINATE BETWEEN IF POINTER OR NOT, DO NECESSARY CHECKS BEFORE USING THIS
        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;
        llvm::Type* getLLVMType(llvm::LLVMContext &ctx) ; //Updates cache
        
        void print() const override {
            std::cout << "STRUCT";
            CType::print();
        }
    };
}