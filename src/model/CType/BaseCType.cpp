#include "BaseCType.h"

using namespace c4::model::ctype;

//Needs to be adjusted if more types are added
bool isCompatible(TypeSpecifier ts1, TypeSpecifier ts2) {
    return (ts1 != TypeSpecifier::VOID && ts2 != TypeSpecifier::VOID) || 
        (ts1 == ts2 && ts1 == TypeSpecifier::VOID);
}

bool BaseCType::equals(const CType* another) const {
    if(another->isFunction) {
        return false;
    }
    //else
    const BaseCType* casted = dynamic_cast<const BaseCType*>(another);
    return (this->indirections == another->indirections) &&
        (indirections == 0 ? isCompatible(this->t, casted->t) : this->t == casted->t);
    
}

Type* BaseCType::getLLVMType(IRBuilder<> &builder) const {
    if(indirections) {
        return builder.getPtrTy();
    }
    switch(t) {
        case TypeSpecifier::INT:
            return builder.getInt32Ty();
        case TypeSpecifier::CHAR:
            return builder.getInt8Ty();
        case TypeSpecifier::VOID: 
            return builder.getVoidTy();
        default:
            throw std::logic_error("Unrecognized type to LLVM-convert to");
    }
    return NULL;
}
