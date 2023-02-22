#pragma once

#include "CType.h"
#include "llvm/IR/DerivedTypes.h"

namespace c4::model::ctype {

    class CFunctionType : public CType {

    public:
        std::shared_ptr<const CType> retType;
        std::vector<std::shared_ptr<const CType>> paramTypes;
        
        CFunctionType(
            std::shared_ptr<const CType> retType,
            const std::vector<std::shared_ptr<const CType>> &paramTypes,
            int indirections
        ) : CType(indirections, FUNCTION), retType(retType), paramTypes(paramTypes) {
            //Fix for f(void)
            if(paramTypes.size() == 1 && paramTypes[0]->isVoid()) {
                this->paramTypes.pop_back();
            }
            //Fix for func designators as parameters
            for(size_t i=0; i<this->paramTypes.size(); i++) {
                if(this->paramTypes[i]->isFuncNonDesignator()) {
                    this->paramTypes[i] = this->paramTypes[i]->addStar();
                }
            } 
        }

        CFunctionType(
            std::shared_ptr<const CType> retType,
            const std::vector<std::shared_ptr<const CType>> &paramTypes
        ) : CFunctionType(retType, paramTypes, 0) {}

        static std::shared_ptr<CFunctionType> get(
            std::shared_ptr<const CType> retType,
            const std::vector<std::shared_ptr<const CType>> &paramTypes
        ) {
            return std::make_shared<CFunctionType>(retType, paramTypes);
        }

        virtual std::shared_ptr<CType> dereference() const override {
            // return nullptr;
            return std::make_shared<CFunctionType>(retType, paramTypes, indirections-1);
        }
        
        virtual std::shared_ptr<CType> addStar() const override {
            // return nullptr;
            return std::make_shared<CFunctionType>(retType, paramTypes, indirections+1);
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
        bool isVoidStar() const override {
            return false;
        }

        bool isComplete() const override {
            return true;
        }

        virtual bool compatible(const CType* another) const override;
        virtual bool assignmentCompatible(const CType* another) const override;
        virtual bool equivalent(const CType* another) const override;
        // bool conflicting(const CFunctionType* another) const;

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;

        llvm::FunctionType* getLLVMFuncType(llvm::LLVMContext &ctx) const;
    };
 
}