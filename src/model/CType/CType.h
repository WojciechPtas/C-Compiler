#pragma once

#include <memory>
#include <iostream>
#include <stdexcept>
//Few declarations to avoid including huge stuff every time
namespace llvm {
    class LLVMContext;
    class Type;
    class Value;
}

namespace c4::model::ctype {

//CTypes represent types in the C languages.
//Objects of this and any subclasses are immutable
class CType {
    protected:
        enum CTypeKind {
            BASE,
            STRUCT,
            FUNCTION
        };
    public:
        const int indirections;
        const CTypeKind kind; 
        CType(int indirections, CTypeKind kind) : indirections(indirections), kind(kind) {
            if(indirections < 0) {
                throw std::logic_error("Decremented indirections below 0!");
            }
        }
        //kind == CTypeKind::STRUCT && !isPointer();
        bool isStruct() const {
            return kind == CTypeKind::STRUCT && !isPointer();
        }
        //kind == CTypeKind::FUNCTION && indirections <= 1;
        bool isFunc() const {
            return kind == CTypeKind::FUNCTION && indirections <= 1;
        }
        //kind == CTypeKind::FUNCTION && indirections == 1;
        bool isFuncDesignator() const {
            return kind == CTypeKind::FUNCTION && indirections == 1;
        }
        //kind == CTypeKind::FUNCTION && indirections == 0;
        bool isFuncNonDesignator() const {
            return kind == CTypeKind::FUNCTION && indirections == 0;
        }

        virtual bool isInteger() const = 0; 
        virtual bool isBool() const = 0; 
        virtual bool isVoid() const = 0;
        virtual bool isVoidStar() const = 0;

        //This does not classify functions as pointer, but does so on function pointers (designators) 
        bool isPointer() const {
            return indirections;
        }
        
        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const = 0;
        virtual bool compatible(const CType* another) const = 0;
        virtual bool equivalent(const CType* another) const { //As compatible, but requires the integer size to be the same
            return this->compatible(another);
        }
        virtual std::shared_ptr<CType> dereference() const = 0;
        virtual std::shared_ptr<CType> addStar() const = 0;

        virtual bool isComplete() const = 0;

        virtual void print() const {
            for (int i = 0; i < indirections; i++) {
                std::cout << "*";
            }
            
        }
    };
}