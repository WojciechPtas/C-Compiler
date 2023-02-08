#include "../../service/CodeGenerator/codegen.h"
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

void CodeGen::dereference(CTypedValue &ctv) {
    ctv.type = ctv.type->dereference();

    //It loads a value of the updated type using the old value as pointer. New value is updated with loaded value.
    ctv.value = builder.CreateLoad(ctv.getLLVMType(ctx), ctv.value);
}

CTypedValue CodeGen::visitLValue(const BinaryExpression &expr) {
    if(expr.type == BinaryExpressionType::Assignment) {
        //Will return exactly as is
        return expr.left->getLValue(*this);
    }
    //else
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
    return scope[expr.identifier];
}
CTypedValue CodeGen::visitLValue(const IndexExpression &expr) {
    // expr.index must be integer
    //expr.container must have lvalue
    CTypedValue idx = expr.index->getRValue(*this);
    CTypedValue base = expr.container->getRValue(*this); //RValue of container should be a pointer
    CTypedValue indexedLValue(nullptr, base.type->dereference());

    std::vector<Value*> indexes;
    indexes.push_back(idx.value);

    //Value of indexedLValue has been dereferenced, but we still want its lvalue
    //GetElementPtr of elements of size "indexedElement", starting at container.value (rvalue!) stepping a number of times dictated by rvalue of idx
    indexedLValue.value = 
        builder.CreateGEP(
            indexedLValue.getLLVMType(ctx),
            base.value,
            indexes
        );
    return indexedLValue;
}

CTypedValue CodeGen::visitLValue(const MemberExpression &expr) {
    CTypedValue base = expr.container->getLValue(*this);

    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const SizeOfType &expr) {
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const UnaryExpression &expr) {
    //We want the L-value of *p: then, we just get the R-value for p.
    if(expr.type == UnaryExpressionType::Indirection) {
        return expr.expression->getRValue(*this);
    }
    //else
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