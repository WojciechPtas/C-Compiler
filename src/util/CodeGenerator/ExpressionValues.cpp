#include "../../service/CodeGenerator/codegen.h"

#include <memory>
#include "../../model/CType/CStructType.h"
#include "../../model/CType/CTypedValue.h"

#include "../../model/expression/BinaryExpression.h"
#include "../../model/expression/CallExpression.h"
#include "../../model/expression/ConditionalExpression.h"
#include "../../model/expression/ConstantExpression.h"
#include "../../model/expression/IdentifierExpression.h"
#include "../../model/expression/IndexExpression.h"
#include "../../model/expression/MemberExpression.h"
#include "../../model/expression/SizeOfType.h"
#include "../../model/expression/UnaryExpression.h"


using namespace llvm;
using namespace c4::model::ctype;

//TODO error handling

// void CodeGen::dereferenceAndLoad(CTypedValue &ctv) {
//     ctv.type = ctv.type->dereference();

//     //It loads a value of the updated type using the old value as pointer. New value is updated with loaded value.
//     ctv.value = builder.CreateLoad(ctv.getLLVMType(ctx), ctv.value);
// }

CTypedValue CodeGen::visitLValue(const BinaryExpression &expr) {
    if(expr.type == BinaryExpressionType::Assignment) {
        //Error if expression doesn't have lvalue AND there was no error before
        CTypedValue lhs = expr.left->getLValue(*this);
        CTypedValue rhs = expr.left->getRValue(*this);
        if(!lhs.isValid() /*&& !error()*/) {
            //set error
            return CTypedValue::invalid();
        }
        if(!rhs.isValid()) {
            return CTypedValue::invalid();
        }
        builder.CreateStore(rhs.value, lhs.value);
        return lhs;
    }
    //else expression has no lvalue
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const CallExpression &expr) {
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const ConditionalExpression &expr) {
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const ConstantExpression &expr) {
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const IdentifierExpression &expr) {
    if(scope.count(expr.identifier)) {
        return scope[expr.identifier];
    }
    else {
        return CTypedValue::invalid();
    }
}
CTypedValue CodeGen::visitLValue(const IndexExpression &expr) {
    CTypedValue base = expr.container->getRValue(*this); //RValue of container should be a pointer
    if(!base.isValid()) {
        //You can always obtain the rvalue of an expression. The only way you get invalid is if there was an error beforehand
        return CTypedValue::invalid();
    }
    if(!base.type->isPointer()) {
        //Not a pointer!
        return CTypedValue::invalid();
    }
    
    CTypedValue idx = expr.index->getRValue(*this);
    if(!idx.type->isInteger()) {
        //idx not an integer!
        return CTypedValue::invalid();
    }
    
    CTypedValue indexedLValue(nullptr, base.type->dereference());

    std::vector<Value*> indexes;
    indexes.push_back(idx.value);

    //Value of indexedLValue has been dereferenced, but we still want its lvalue
    //GetElementPtr of elements of size "indexedElement", starting at base.value (rvalue!), stepping idx times
    indexedLValue.value = 
        builder.CreateGEP(
            indexedLValue.getLLVMType(ctx),
            base.value,
            indexes
        );
    return indexedLValue;
}

CTypedValue CodeGen::visitLValue(const MemberExpression &expr) {
    CTypedValue base;
    //must have correct type
    if(expr.type == MemberExpressionType::Direct) {
        base = expr.container->getLValue(*this);
        if(!base.isValid()) {
            // if(!error()) {
            //     accessing something with no lvalue
            // }
            return CTypedValue::invalid();
        }
    }
    else {//Pointer. As for dereferencing, we take its R-value
        base = expr.container->getRValue(*this);
        if(!base.isValid()) {
            return CTypedValue::invalid();
        }

        if(!base.type->isPointer()) {
            //Dereferencing a non-pointer
            return CTypedValue::invalid();
        }

        base.dereference();
    }
    
    if(!base.type->isStruct()) {
        //set error is not a struct
        return CTypedValue::invalid();
    }

    auto structType = std::dynamic_pointer_cast<const CStructType, const CType>(base.type);
    std::string memberName = expr.member->identifier;
    
    if(!structType->hasMember(memberName)) {
        //set error has no member with name
        return CTypedValue::invalid();
    }

    uint memberIndex = structType->getIndexOf(memberName);

    std::vector<Value*> indexes;
    indexes.push_back(builder.getInt32(0));
    indexes.push_back(builder.getInt32(memberIndex));

    Value* value = builder.CreateInBoundsGEP(base.getLLVMType(ctx), base.value, indexes);
    std::shared_ptr<const CType> type = structType->fieldTypes[memberIndex];
    return CTypedValue(value, type);
    
}
CTypedValue CodeGen::visitLValue(const SizeOfType &expr) {
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const UnaryExpression &expr) {
    //We want the L-value of *p: then, we just get the R-value for p.
    //Check for indirections: at least 1
    if(expr.type == UnaryExpressionType::Indirection) {
        CTypedValue ptr = expr.expression->getRValue(*this);
        if(!ptr.isValid()) {
            return CTypedValue::invalid();
        }

        if(!ptr.type->isPointer()) {
            //dereferenced something that's not a pointer
            return CTypedValue::invalid();
        }

        ptr.dereference();
        return ptr; //Not a ptr anymore
    }
    //else this kind of expression has no lvalue
    return CTypedValue::invalid();
}



//Integer addition AND pointer addition! Che palle...
CTypedValue CodeGen::visitRValue(const BinaryExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const CallExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const ConditionalExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const ConstantExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const IdentifierExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const IndexExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const MemberExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const SizeOfType &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const UnaryExpression &expr) {
    // switch(expr.type) {
    //     case UnaryExpressionType::Indirection:
    //         CTypedValue ptr = expr.expression->getRValue(*this);
    //         ptr
    // }
    return CTypedValue::invalid();
}