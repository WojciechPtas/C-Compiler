#pragma once

#include <memory>

//Few declarations to avoid including huge stuff every time
namespace llvm {
    class LLVMContext;
    class Type;
    class Value;
}

namespace c4::model::ctype {

enum TypeSpecifier {
    VOID,
    INT,
    CHAR
};

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
        virtual llvm::Type* getLLVMType(llvm::LLVMContext &ctx) const = 0;
        virtual bool compatible(const CType* another) const = 0;
        virtual std::shared_ptr<const CType> dereference() const = 0;
        virtual std::shared_ptr<const CType> addStar() const = 0;
    };
}