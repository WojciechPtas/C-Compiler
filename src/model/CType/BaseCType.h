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

        virtual std::shared_ptr<CType> dereference() const override {
            return BaseCType::get(t, indirections-1);
        }
        
        virtual std::shared_ptr<CType> addStar() const override {
            return BaseCType::get(t, indirections+1);
        }

        static std::shared_ptr<BaseCType> get(TypeSpecifier ts, int indirections) {
            return std::make_shared<BaseCType>(ts, indirections);
        }
        static std::shared_ptr<BaseCType> get(TypeSpecifier ts) {
            return BaseCType::get(ts, 0);
        }

        bool isInteger() const override {
            return (indirections == 0) && ((t == TypeSpecifier::INT) || (t == TypeSpecifier::CHAR) || (t == TypeSpecifier::BOOL));
        }

        bool isBool() const override {
            return (t == TypeSpecifier::BOOL) && (indirections == 0);
        }

        bool isVoid() const override {
            return (t == TypeSpecifier::VOID) && (indirections == 0);
        }

        bool isComplete() const override {
            return true;
        }


        virtual bool compatible(const CType* another) const override;
        virtual bool equivalent(const CType* another) const override;

        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const override;

        virtual void print() const override {
            switch(t) {
                case TypeSpecifier::BOOL: {
                    std::cout << "bool";
                    break;
                }
                case TypeSpecifier::CHAR: {
                    std::cout << "char";
                    break;
                }
                case TypeSpecifier::INT: {
                    std::cout << "int";
                    break;
                }
                case TypeSpecifier::VOID : {
                    std::cout << "void";
                    break;
                }
                default: {

                }
            }
            this->CType::print();
        }
    };
}