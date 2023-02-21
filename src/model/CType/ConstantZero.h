#pragma once

#include "CType.h"

namespace c4::model::ctype {
    class ConstantZero : public CType {

    public:
        ConstantZero() : CType(0, CONSTANTZERO) {}
        
        virtual bool isPointer() const override {
            return true;
        }
        virtual bool isInteger() const {
            return false;
        } 
        virtual bool isBool() const {
            return false;
        }

        virtual bool isVoid() const {
            return false;
        }
        virtual bool isVoidStar() const {
            return true;
        }
        virtual bool isComplete() const {
            return true;
        }

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override {
            (void) ctx;
            throw std::logic_error("Called getLLVMType() on a ConstantZero");
            return nullptr;
        }
        virtual bool compatible(const CType* another) const override {
            (void) another;
            throw std::logic_error("Called compatible() on a ConstantZero");
            return false;
        }
        virtual bool assignmentCompatible(const CType* another) const override {
            (void) another;

            throw std::logic_error("Called assignmentCompatible() on a ConstantZero");
            return false;
        }
        bool equivalent(const CType* another) const override {
            (void) another;

            throw std::logic_error("Called equivalent() on a ConstantZero");
            return false;
        }
        std::shared_ptr<CType> dereference() const override {
            throw std::logic_error("Called dereference() on a ConstantZero");
            return nullptr;
        }
        std::shared_ptr<CType> addStar() const override {
            throw std::logic_error("Called addStar() on a ConstantZero");
            return nullptr;
        }

        static std::shared_ptr<ConstantZero> get() {
            return std::make_shared<ConstantZero>();
        }
    };
}