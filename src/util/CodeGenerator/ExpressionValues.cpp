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

/*
CTypedValues of type function can have either 0 or 1 indirections (functions or function designators)
Functions can either be called(getLValue(), which works for both since they are the same: pointers to a code section) or evaluated(getRValue(), which should convert to function object and use getLvalue())
Plain functions (not designators) are not assignable
Pointer arithmetic on functions should work the same, and functions designators will be converted to functions
I'd say: don't allow function pointer operations at all. If needed, a bitcast should do the job

Dereferencing a function must yield its lvalue (as everything that asks for its rvalue should do) and its type must not be dereferenced
*/

//TODO error handling
//Need to check if it's a function, to treat it accordingly
//For integers, rvalue loading should produce an int
//For integers, be sure the value is truncated accordingly if necessary before storing

// void CodeGen::dereferenceAndLoad(CTypedValue &ctv) {
//     ctv.type = ctv.type->dereference();

//     //It loads a value of the updated type using the old value as pointer. New value is updated with loaded value.
//     ctv.value = builder.CreateLoad(ctv.getLLVMType(ctx), ctv.value);
// }

//Will use the GEP instruction with side effect to base, but its type is not changed
void CodeGen::pointerAddInt(CTypedValue &base, const CTypedValue &index) {
    std::vector<Value*> indices;
    indices.push_back(index.value);
    base.value = builder.CreateGEP(
        base.type->dereference()->getLLVMType(ctx),
        base.value,
        indices
    );
}

void CodeGen::unifyIntegerSize(CTypedValue &lhs, CTypedValue &rhs) {
    Type* leftType = lhs.value->getType();
    Type* rightType = rhs.value->getType();
    uint leftBits = leftType->getIntegerBitWidth();
    uint rightBits = leftType->getIntegerBitWidth();

    if(leftBits > rightBits) {
        rhs.value = builder.CreateSExt(
            rhs.value,
            IntegerType::getIntNTy(ctx, leftBits)
        );
        rhs.type = lhs.type;
    }
    else if(rightBits > leftBits) {
        lhs.value = builder.CreateSExt(
            lhs.value,
            IntegerType::getIntNTy(ctx, rightBits)
        );
        lhs.type = rhs.type;
    }
}

Value* CodeGen::ptrToInt64(Value* value) {
    value = builder.CreatePtrToInt(
        value,
        IntegerType::getInt64Ty(ctx)
    );
    return value;
}

Value* CodeGen::intToBool(Value* value) {
    uint bitwidth = value->getType()->getIntegerBitWidth();
    value = builder.CreateICmpNE(value, builder.getIntN(bitwidth, 0));
    return value;
}

//Helper function to generate code that checks if an integer/pointer is equal to 0
Value* CodeGen::evaluateCondition(CTypedValue& ctv) {
    Value* condition;

    if(ctv.type->isPointer() && !ctv.type->isFunc()) {
        ctv.value = ptrToInt64(ctv.value);
        ctv.type = BaseCType::get(TypeSpecifier::INT); //Sets up for the following condition
    }

    if(ctv.type->isInteger()) {
        if(!ctv.type->isBool()) {
            condition = intToBool(ctv.value);
        }
        else {
            condition = ctv.value;
        }
    }
    else {
        //Rvalue must have integer or pointer type
        return nullptr;
    }
    return condition;
}

CTypedValue CodeGen::visitLValue(const BinaryExpression &expr) {
    if(expr.type == BinaryExpressionType::Assignment) {
        //Error if expression doesn't have lvalue AND there was no error before
        CTypedValue lhs = expr.left->getLValue(*this);
        CTypedValue rhs = expr.right->getRValue(*this);
        if(!lhs.isValid()) {
            // if(!error()) {
            //     set error

            // }
            return CTypedValue::invalid();
        }
        if(!rhs.isValid()) {
            return CTypedValue::invalid();
        }

        if(!lhs.type->compatible(rhs.type.get())) {
            //Incompatible types
            return CTypedValue::invalid();
        }

        if(lhs.isConst()) {
            //Lvalue is not modifiable
            return CTypedValue::invalid();
        }


        builder.CreateStore(rhs.value, lhs.value);
        return lhs;
    }
    //else expression has no lvalue
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const CallExpression &expr) {
    //has no lvalue
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const ConditionalExpression &expr) {
    //has no lvalue
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const ConstantExpression &expr) {
    //has no lvalue
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const IdentifierExpression &expr) {
    if(scope.count(expr.identifier)) {
        return scope[expr.identifier];
    }
    else {
        //Undeclared
        return CTypedValue::invalid();
    }
}
CTypedValue CodeGen::visitLValue(const IndexExpression &expr) {
    CTypedValue base = expr.container->getRValue(*this); //RValue of container should be a pointer
    if(!base.isValid()) {
        //You can always obtain the rvalue of an expression. The only way you get invalid is if there was an error beforehand
        return CTypedValue::invalid();
    }
    if(base.type->isFunc()) {
        //Function pointer arithmetic not allowed
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

    pointerAddInt(base, idx);
    base.dereference();
    //Value of indexedLValue has been dereferenced, but we still want its lvalue
    return base;
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

    Value* value = builder.CreateStructGEP(base.getLLVMType(ctx), base.value, memberIndex);
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
        if(!ptr.type->isFuncNonDesignator()) {
            ptr.dereference();
        }
        return ptr; //Not a ptr anymore
    }
    //else this kind of expression has no lvalue
    return CTypedValue::invalid();
}



//Integer addition AND pointer addition! Che palle...
CTypedValue CodeGen::visitRValue(const BinaryExpression &expr) {
    if (expr.type == BinaryExpressionType::Assignment) {
        //Source of inefficiency, but at least it allows for good code recycling
        //Since assignments have lvalue, and getting the lvalue computes side effect, to get the rvalue just load from its lvalue
        CTypedValue rvalue = expr.getLValue(*this);
        rvalue.value = builder.CreateLoad(rvalue.getLLVMType(ctx), rvalue.value);
        return rvalue;
    }

    CTypedValue lhs = expr.left->getRValue(*this);
    CTypedValue rhs = expr.right->getRValue(*this);
    if(!(lhs.isValid() && rhs.isValid())) {
        return CTypedValue::invalid();
    }

    //Following are lazy-evaluating operators
    switch(expr.type) {
        case BinaryExpressionType::LogicalOr:
            //Continue with logicalAnd
        case BinaryExpressionType::LogicalAnd: {
            Value* leftCond = evaluateCondition(lhs);
            if(leftCond == nullptr) {
                //Error encountered and reported
                return CTypedValue::invalid();
            }

            BasicBlock* leftEvalBlock = builder.GetInsertBlock();
            Function* currentFunction = leftEvalBlock->getParent();

            BasicBlock* lazyEvalBlock = BasicBlock::Create(
                ctx,
                "LogicalAndLazyEval",
                currentFunction
            );

            BasicBlock* endBlock = BasicBlock::Create(
                ctx,
                "LogicalAndEnd",
                currentFunction
            );

            if(expr.type == BinaryExpressionType::LogicalAnd) {
                builder.CreateCondBr(leftCond, lazyEvalBlock, endBlock);
            }
            else {
                builder.CreateCondBr(leftCond, endBlock, lazyEvalBlock);
            }

            //Now we insert code that evaluates rhs only if lhs was true
            builder.SetInsertPoint(lazyEvalBlock);

            Value* rightCond = evaluateCondition(rhs);
            if(rightCond == nullptr) {
                //Error encountered and reported
                return CTypedValue::invalid();
            }
            Value* result = (expr.type == BinaryExpressionType::LogicalAnd) ?
                builder.CreateAnd(leftCond, rightCond) :
                builder.CreateOr(leftCond, rightCond);
            builder.CreateBr(endBlock);

            //And now we combine the two branches with a phi node
            builder.SetInsertPoint(endBlock);
            PHINode* phi = builder.CreatePHI(IntegerType::getInt1Ty(ctx), 2);
            phi->addIncoming(
                (expr.type == BinaryExpressionType::LogicalAnd) ? builder.getFalse() : builder.getTrue(), 
                leftEvalBlock);
            phi->addIncoming(result, lazyEvalBlock);

            return CTypedValue(
                phi,
                BaseCType::get(TypeSpecifier::BOOL)
            );
        }
    
        case BinaryExpressionType::Unequal:
        case BinaryExpressionType::LessThan:
        case BinaryExpressionType::Equal: {
            CTypedValue lhs = expr.left->getRValue(*this);
            CTypedValue rhs = expr.right->getRValue(*this);

            if(!(lhs.isValid() && rhs.isValid())) {
                return CTypedValue::invalid();
            }

            if(!lhs.compatible(rhs)) {
                //Comparison of incompatible type
                return CTypedValue::invalid();
            }

            if(lhs.type->isFunc()) {
                //Function pointer comparison
                return CTypedValue::invalid();
            }
            
            if(lhs.type->isPointer()) { //They're both pointers and of the same type
                lhs.value = ptrToInt64(lhs.value);
                lhs.type = BaseCType::get(TypeSpecifier::INT); //Sets up for the following condition
                rhs.value = ptrToInt64(rhs.value);
                rhs.type = lhs.type; //Sets up for the following condition
            }

            else if(!lhs.type->isInteger()) {
                //Comparison cannot be done with values of this type
                return CTypedValue::invalid();
            }
            
            unifyIntegerSize(lhs, rhs);
            CTypedValue result(nullptr, std::make_shared<BaseCType>(TypeSpecifier::BOOL));
            switch(expr.type) {
                case BinaryExpressionType::Equal: {
                    result.value = builder.CreateICmpEQ(
                        lhs.value,
                        rhs.value
                    );
                    break;
                }
                case BinaryExpressionType::Unequal: {
                    result.value = builder.CreateICmpNE(
                        lhs.value,
                        rhs.value
                    );
                    break;
                }
                case BinaryExpressionType::LessThan: {
                    result.value = builder.CreateICmpSLT(
                        lhs.value,
                        rhs.value
                    );
                    break;
                }
                default:
                    throw std::logic_error("Inner switch for code generation for comparisons dealt with an unexpected type");
            }
            return result;
        }

        case BinaryExpressionType::Sum:
        case BinaryExpressionType::Subtraction:
        case BinaryExpressionType::Multiplication: {
            CTypedValue lhs = expr.left->getRValue(*this);
            CTypedValue rhs = expr.right->getRValue(*this);

            if(!(lhs.isValid() && rhs.isValid())) {
                return CTypedValue::invalid();
            }

            if(lhs.type->isFunc() || rhs.type->isFunc()) {
                //Function pointer comparison
                return CTypedValue::invalid();
            }

            CTypedValue result;

            if(lhs.type->isInteger() && rhs.type->isInteger()) {
                unifyIntegerSize(lhs, rhs);
                result.type = lhs.type;
                switch(expr.type) {
                    case BinaryExpressionType::Sum: {
                        result.value = builder.CreateAdd(lhs.value, rhs.value);
                        break;
                    }
                    case BinaryExpressionType::Subtraction: {
                        result.value = builder.CreateSub(lhs.value, rhs.value);
                        break;
                    }
                    case BinaryExpressionType::Multiplication: {
                        result.value = builder.CreateMul(lhs.value, rhs.value);
                        break;
                    }
                    default:
                        throw std::logic_error("Inner switch for code generation for arithmetic dealt with an unexpected type");
                }
            }
            else if(lhs.type->isPointer() && rhs.type->isInteger()) {
                if(expr.type == BinaryExpressionType::Multiplication) {
                    //Multiplication of pointer with an integer
                    return CTypedValue::invalid();
                }
                else if(expr.type == BinaryExpressionType::Subtraction) {
                    rhs.value = builder.CreateNeg(rhs.value);
                }
                pointerAddInt(lhs, rhs);
                return lhs;
            }
            else if(lhs.type->isInteger() && rhs.type->isPointer()) {
                if(expr.type == BinaryExpressionType::Multiplication) {
                    //Multiplication of pointer with an integer
                    return CTypedValue::invalid();
                }
                else if(expr.type == BinaryExpressionType::Subtraction) {
                    //Subtracting a pointer from an integer
                    return CTypedValue::invalid();
                }
                pointerAddInt(rhs, lhs);
                return rhs;
            }
            else if(lhs.type->isPointer() && lhs.compatible(rhs)) {
                if(expr.type == BinaryExpressionType::Subtraction) {
                    return CTypedValue(
                        builder.CreatePtrDiff(
                            lhs.type->dereference()->getLLVMType(ctx),
                            lhs.value,
                            rhs.value
                        ),
                        BaseCType::get(TypeSpecifier::INT)
                    );
                }
                else {
                    //Non-sub arithmetic not allowed between pointers
                    return CTypedValue::invalid();
                }
            }
            else {
                //Operator in incompatible with the operands
                return CTypedValue::invalid();
            }
        }
    }
    
}
CTypedValue CodeGen::visitRValue(const CallExpression &expr) {
    //In this context we need to invoke the function. But first we need the function pointer
    CTypedValue called = expr.called->getRValue(*this);
    if(!called.isValid()) {
        // if(!error()) {
        //     called something with no lvalue
        // }
        return CTypedValue::invalid();
    }

    if(!called.type->isFunc()) {
        //Calling something that's not a function
        return CTypedValue::invalid(); 
    }
    auto funcType = std::dynamic_pointer_cast<const CFunctionType, const CType>(called.type);
    if(expr.arguments.arguments.size() != funcType->paramTypes.size()) {
        //Different number of parameters
        return CTypedValue::invalid();
    }

    std::vector<Value*> llvmArguments;

    for(uint i=0; i<funcType->paramTypes.size(); i++) {
        //I take the Rvalue of one of the arguments in the expression
        CTypedValue argRvalue = expr.arguments.arguments[i]->getRValue(*this);
        if(!argRvalue.isValid()) {
            return CTypedValue::invalid();
        }
        //I check if they have compatible types
        if(!funcType->paramTypes[i]->compatible(argRvalue.type.get())) {
            //Incompatible types in function call
            return CTypedValue::invalid();
        }
        //If all is good, i fill a vector of Value* for the llvm Call instruction
        llvmArguments.push_back(argRvalue.value);
    }
    CTypedValue returnedRValue;
    returnedRValue.type = funcType->retType;
    //Following static_cast is safe under the assumption that if called.isValid() && called.type->isFunction(), then called.value will contain a Function*
    returnedRValue.value = builder.CreateCall(static_cast<Function*>(called.value), llvmArguments);

    return returnedRValue;

}
CTypedValue CodeGen::visitRValue(const ConditionalExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const ConstantExpression &expr) {
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const IdentifierExpression &expr) {
    CTypedValue lvalue = expr.getLValue(*this);
    //Checks for existence are already done
    if(lvalue.type->isFuncNonDesignator()) {
        return lvalue;
    }
    else {
        lvalue.value = builder.CreateLoad(lvalue.getLLVMType(ctx), lvalue.value);
        return lvalue;
    }
}
CTypedValue CodeGen::visitRValue(const IndexExpression &expr) {
    CTypedValue lvalue = expr.getLValue(*this);
    //Checks already done
    if(lvalue.type->isFuncNonDesignator()) {
        //Shouldn't need this at the moment, as function pointer arithmetic is not yet supported
        return lvalue;
    }
    else {
        lvalue.value = builder.CreateLoad(lvalue.getLLVMType(ctx), lvalue.value);
        return lvalue;
    }
    
}
CTypedValue CodeGen::visitRValue(const MemberExpression &expr) {
    CTypedValue lvalue = expr.getLValue(*this);
    //Checks already done
    if(lvalue.type->isFuncNonDesignator()) {
        //Shouldn't need this at the moment, as function pointer arithmetic is not yet supported
        return lvalue;
    }
    else {
        lvalue.value = builder.CreateLoad(lvalue.getLLVMType(ctx), lvalue.value);
        return lvalue;
    }
    
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