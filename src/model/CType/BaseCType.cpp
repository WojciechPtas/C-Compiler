#include "BaseCType.h"
#include "llvm/IR/DerivedTypes.h"

using namespace c4::model::ctype;
using namespace llvm;

bool isVoidStar(const CType* ctype) {
    return ctype->isVoid() && ctype->indirections == 1;
}

bool BaseCType::compatible(const CType* another) const {
    if(this == another) { //Same address!
         return true;
    }
    //special case of void*
    else if(
        (isVoidStar(this) && (another->isPointer() || another->isFunc())) || 
        (isVoidStar(another) && (this->isPointer() || this->isFunc()))
    ) {
        return true;
    }
    else if(this->kind != another->kind) {
        return false;
    }
    //else
    const BaseCType* casted = dynamic_cast<const BaseCType*>(another);
    return (this->indirections == another->indirections) &&
        (this->t == casted->t || 
            (indirections == 0 && this->isInteger() && casted->isInteger())
        );
}

bool BaseCType::equivalent(const CType* another) const { //As compatible, but requires the integer size to be the same
    if(this->compatible(another)) {
        auto casted = dynamic_cast<const BaseCType*>(another);
        return this->t == casted->t;
    }
    else {
        return false;
    }
}

Type* BaseCType::getLLVMType(LLVMContext &ctx) const {
    if(indirections) {
        return PointerType::getUnqual(ctx);
    }
    switch(t) {
        case TypeSpecifier::INT:
            return IntegerType::getInt32Ty(ctx);
        case TypeSpecifier::CHAR:
            return IntegerType::getInt8Ty(ctx);
        case TypeSpecifier::VOID: 
            return Type::getVoidTy(ctx);
        default:
            throw std::logic_error("Unrecognized type to LLVM-convert to");
    }
    return NULL;
}
