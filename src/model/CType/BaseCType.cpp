#include "BaseCType.h"
#include "llvm/IR/DerivedTypes.h"

using namespace c4::model::ctype;
using namespace llvm;

bool BaseCType::compatible(const CType* another) const {
    if(this == another) { //Same address!
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

bool BaseCType::assignmentCompatible(const CType* another) const {
    return this->compatible(another) || 
    (this->isVoidStar() && (another->isPointer() || another->isFunc())) || 
        (another->isVoidStar() && (this->isPointer() || this->isFunc()));
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
