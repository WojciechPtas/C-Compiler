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

#include "../lexer/ReadUtilities.h"

using namespace llvm;
using namespace c4::model::ctype;
using c4::model::ConstantType;
using namespace c4::model::expression;
/*
CTypedValues of type function can have either 0 or 1 indirections (functions or function designators)
Functions can either be called(getRValue(), which works for both since they are the same: pointers to a code section) or evaluated(still getRValue(), which should convert to function object and use getLvalue())
Plain functions (not designators) are not assignable: getLValue() returns the same as getRValue() for functionNonDesignators, but they can't be modified(a check is made for assignments)
Pointer arithmetic on functions is not allowed, but function pointer comparisons work

Dereferencing a function must yield its lvalue (as everything that asks for its rvalue should do) and its type must not be dereferenced
*/


// void CodeGen::dereferenceAndLoad(CTypedValue &ctv) {
//     ctv.type = ctv.type->dereference();

//     //It loads a value of the updated type using the old value as pointer. New value is updated with loaded value.
//     ctv.value = builder.CreateLoad(ctv.getLLVMType(ctx), ctv.value);
// }

void CodeGen::convertToINT(CTypedValue& ctv) { //No checks performed. Argument must be integer type.
    ctv.type = BaseCType::get(TypeSpecifier::INT);
    ctv.value = builder.CreateSExtOrTrunc(
        ctv.value,
        IntegerType::getInt32Ty(ctx)
    );
}

CTypedValue CodeGen::loadFromLValue(const IExpression& expr) {
    //Source of inefficiency, but at least it allows for good code recycling
    //For assignments, since they have lvalue, and getting the lvalue computes side effect, to get the rvalue just load from its lvalue
    CTypedValue rvalue = expr.getLValue(*this);
    if(!rvalue.isValid()) {
        return CTypedValue::invalid();
    }

    if(!rvalue.type->isFuncNonDesignator()) {
        rvalue.value = builder.CreateLoad(rvalue.getLLVMType(ctx), rvalue.value);
    }
    return rvalue;
}

Value* CodeGen::funcToPtr(Value* func) { //No checks performed. Must be function.
    return builder.CreateBitCast(func, PointerType::getUnqual(ctx));
}

//No checks performed.
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

//No checks performed. Both are assumed to be of integer type.
void CodeGen::unifyIntegerSize(CTypedValue &lhs, CTypedValue &rhs, BasicBlock* insertLeftHere, BasicBlock* insertRightHere) {
    BasicBlock* oldInsertPoint = builder.GetInsertBlock();
    Type* leftType = lhs.type->getLLVMType(ctx);
    Type* rightType = rhs.type->getLLVMType(ctx);
    uint leftBits = leftType->getIntegerBitWidth();
    uint rightBits = rightType->getIntegerBitWidth();

    if(leftBits > rightBits) {
        builder.SetInsertPoint(insertRightHere);
        rhs.value = builder.CreateSExt(
            rhs.value,
            IntegerType::getIntNTy(ctx, leftBits)
        );
        rhs.type = lhs.type;
    }
    else if(rightBits > leftBits) {
        builder.SetInsertPoint(insertLeftHere);
        lhs.value = builder.CreateSExt(
            lhs.value,
            IntegerType::getIntNTy(ctx, rightBits)
        );
        lhs.type = rhs.type;
    }
    builder.SetInsertPoint(oldInsertPoint);
}

void CodeGen::unifyIntegerSize(CTypedValue &lhs, CTypedValue &rhs) {
    unifyIntegerSize(lhs, rhs, builder.GetInsertBlock(), builder.GetInsertBlock());
}

//No checks performed. Must be a ptr.
Value* CodeGen::ptrToInt64(Value* value) {
    value = builder.CreatePtrToInt(
        value,
        IntegerType::getInt64Ty(ctx)
    );
    return value;
}

//No checks performed. Must be an integer
Value* CodeGen::intToBool(Value* value, bool negated) {
    uint bitwidth = value->getType()->getIntegerBitWidth();
    if(negated) {
        value = builder.CreateICmpEQ(value, builder.getIntN(bitwidth, 0));
    }
    else {
        value = builder.CreateICmpNE(value, builder.getIntN(bitwidth, 0));
    }
    return value;
}

//Generate code that checks if an integer/pointer is equal to 0. If errors are encountered, it's invalid.
//Checks included :)
void CodeGen::evaluateCondition(CTypedValue& ctv, bool negated=false) {
    if(ctv.isValid()) {
        if(ctv.type->isFunc()) {
            ctv.value = funcToPtr(ctv.value);
        }

        if(ctv.type->isPointer() || ctv.type->isFunc()) {
            ctv.value = ptrToInt64(ctv.value);
            ctv.type = BaseCType::get(TypeSpecifier::INT);
        }

        if(ctv.type->isInteger()) {
            if(!ctv.type->isBool()) {
                ctv.value = intToBool(ctv.value, negated);
                ctv.type = BaseCType::get(TypeSpecifier::BOOL);
            }
        }
        else {
            //"Condition is not scalar!"
            ctv = CTypedValue::invalid();
        }
    }
    //else it will will remain invalid
}

CTypedValue CodeGen::visitLValue(const BinaryExpression &expr) {
    if(expr.type == BinaryExpressionType::Assignment) {
        //Error if expression doesn't have lvalue AND there was no error before
        CTypedValue lhs = expr.left->getLValue(*this);
        CTypedValue rhs = expr.right->getRValue(*this);
        if(!lhs.isValid()) {
            return CTypedValue::invalid();
        }
        if(!rhs.isValid()) {
            return CTypedValue::invalid();
        }

        if(lhs.isConst()) {
            //Lvalue is not modifiable
            return CTypedValue::invalid();
        }

        if(!lhs.type->compatible(rhs.type.get())) {
            //Incompatible types
            return CTypedValue::invalid();
        }

        if(lhs.type->isInteger()) {
            //Need to unify their sizes
            //It would be nice to issue a warning in this case
            rhs.value = builder.CreateSExtOrTrunc(
                rhs.value,
                lhs.type->getLLVMType(ctx)
            );
            rhs.type = lhs.type;
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
    if(scope.isVarDeclared(expr.identifier)) {
        return scope[expr.identifier];
    }
    else {
        //Undeclared
        return CTypedValue::invalid();
    }
}
CTypedValue CodeGen::visitLValue(const IndexExpression &expr) {
    CTypedValue base = expr.container->getRValue(*this); //RValue of container should be a pointer
    CTypedValue idx = expr.index->getRValue(*this);
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
    
    if(!idx.isValid()) {
        return CTypedValue::invalid(); 
    }
    if(!idx.type->isInteger()) {
        //idx not an integer!
        return CTypedValue::invalid();
    }

    pointerAddInt(base, idx);
    base.dereference();
    //Type of indexedLValue has been dereferenced, but we still want its lvalue
    return base;
}

CTypedValue CodeGen::visitLValue(const MemberExpression &expr) {
    CTypedValue base;
    if(expr.type == MemberExpressionType::Direct) {
        base = expr.container->getLValue(*this);
        if(!base.isValid()) {
            return CTypedValue::invalid();
        }
    }
    else {//Pointer. As for dereferencing, we take its R-value
        base = expr.container->getRValue(*this);
        if(!base.isValid()) {
            return CTypedValue::invalid();
        }

        if(!base.type->isPointer()) {
            //Dereferencing a non-struct-pointer
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

        if(!(ptr.type->isPointer() || ptr.type->isFunc())) {
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


CTypedValue CodeGen::visitRValue(const BinaryExpression &expr) {
    if (expr.type == BinaryExpressionType::Assignment) {
        return loadFromLValue(expr);
    }

    //Following are lazy-evaluating operators
    switch(expr.type) {
        case BinaryExpressionType::LogicalOr:
            //Continue with logicalAnd
        case BinaryExpressionType::LogicalAnd: {
            CTypedValue lhs = expr.left->getRValue(*this);
            evaluateCondition(lhs);
            if(!lhs.isValid()) {
                //Error encountered and reported
                return CTypedValue::invalid();
            }
            Value* leftCond = lhs.value;

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
               CTypedValue rhs = expr.right->getRValue(*this);
            evaluateCondition(rhs);
            if(!rhs.isValid()) {
                //Error encountered and reported
                return CTypedValue::invalid();
            }
            Value* rightCond = rhs.value;
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
                lhs.value = funcToPtr(lhs.value);
                rhs.value = funcToPtr(rhs.value);
            }
            
            if(lhs.type->isPointer() || lhs.type->isFunc()) { //They're both pointers and of the same type
                lhs.value = ptrToInt64(lhs.value);
                lhs.type = BaseCType::get(TypeSpecifier::INT);
                rhs.value = ptrToInt64(rhs.value);
                rhs.type = lhs.type;
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
                //Function pointer arithmetic
                return CTypedValue::invalid();
            }


            if(lhs.type->isInteger() && rhs.type->isInteger()) {
                CTypedValue result;
                convertToINT(lhs);
                convertToINT(rhs);
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
                return result;
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
        default:
            throw std::logic_error("Codegen: unrecognized binary expression type");
            return CTypedValue::invalid();
    }
    
}
CTypedValue CodeGen::visitRValue(const CallExpression &expr) {
    //In this context we need to invoke the function. But first we need the function pointer
    CTypedValue called = expr.called->getRValue(*this); //GetRValue() on a function object returns the pointer itself
    if(!called.isValid()) {
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
        if(argRvalue.type->isInteger()) {
            //Need to convert it into the correct size
            //It would be nice to issue a warning in case of trunc, but whatever
            argRvalue.value = builder.CreateSExtOrTrunc(
                argRvalue.value,
                funcType->paramTypes[i]->getLLVMType(ctx)
            );
            argRvalue.type = funcType->paramTypes[i];
        }
        //If all is good, i fill a vector of Value* for the llvm Call instruction
        llvmArguments.push_back(argRvalue.value);
    }
    CTypedValue returnedRValue;
    returnedRValue.type = funcType->retType;
    returnedRValue.value = builder.CreateCall(
        funcType->getLLVMFuncType(ctx), 
        called.value, 
        llvmArguments
    );

    return returnedRValue;

}
CTypedValue CodeGen::visitRValue(const ConditionalExpression &expr) {
    CTypedValue cond = expr.condition->getRValue(*this);
    evaluateCondition(cond);
    if(!cond.isValid()) {
        return CTypedValue::invalid();
    }

    Function* currentFunction = builder.GetInsertBlock()->getParent();

    BasicBlock* leftEvalBlock = BasicBlock::Create(
        ctx,
        "ConditionalExprLeftEval",
        currentFunction
    );
    BasicBlock* rightEvalBlock = BasicBlock::Create(
        ctx,
        "ConditionalExprRightEval",
        currentFunction
    );
    BasicBlock* endBlock = BasicBlock::Create(
        ctx,
        "ConditionalExprEnd",
        currentFunction
    );

    builder.SetInsertPoint(leftEvalBlock);
    CTypedValue leftExpr = expr.thenCase->getRValue(*this);
    builder.CreateBr(endBlock);
    builder.SetInsertPoint(rightEvalBlock);
    CTypedValue rightExpr = expr.thenCase->getRValue(*this);
    builder.CreateBr(endBlock);

    if(!(leftExpr.isValid() && rightExpr.isValid())) {
        return CTypedValue::invalid();
    }

    if(!leftExpr.compatible(rightExpr)) {
        //Left and right of Conditional Expressions not compatible
        return CTypedValue::invalid();
    }

    if(!leftExpr.type->equivalent(leftExpr.type.get()) && leftExpr.type->isInteger()) { //Integers of different sizes
        builder.SetInsertPoint(leftEvalBlock);
        leftExpr.value = builder.CreateSExtOrTrunc(
            leftExpr.value, 
            IntegerType::getInt32Ty(ctx)
        );
        builder.SetInsertPoint(rightEvalBlock);
        rightExpr.value = builder.CreateSExtOrTrunc(
            rightExpr.value, 
            IntegerType::getInt32Ty(ctx)
        );
    }

    //Now they're of the same llvm type
    builder.SetInsertPoint(endBlock);
    PHINode* phi = builder.CreatePHI(
        leftExpr.getLLVMType(ctx),
        2,
        "ConditionalExprChoose"
    );
    phi->addIncoming(
        leftExpr.value,
        leftEvalBlock
    );
    phi->addIncoming(
        rightExpr.value,
        rightEvalBlock
    );

    return CTypedValue(
        phi,
        leftExpr.type //Should both have same type at this point
    );
    
    
}
CTypedValue CodeGen::visitRValue(const ConstantExpression &expr) {
    switch(expr.type) {
        case ConstantType::Character: {
            return CTypedValue(
                builder.getInt8(c4::util::lexer::parseSingleChar(expr.value)),
                BaseCType::get(TypeSpecifier::CHAR)
            );
        }
        case ConstantType::Decimal: {
            return CTypedValue(
                builder.getInt32(std::stoi(expr.value)),
                BaseCType::get(TypeSpecifier::INT)
            );
        }
        case ConstantType::String: {
            return CTypedValue(
                builder.CreateGlobalStringPtr(expr.value),
                BaseCType::get(TypeSpecifier::CHAR, 1)
            );
        }
        default: {
            throw std::logic_error("ConstantExpression CodeGen: unrecognized constant type");
        }
    }
    
}
CTypedValue CodeGen::visitRValue(const IdentifierExpression &expr) {
    return loadFromLValue(expr);
}
CTypedValue CodeGen::visitRValue(const IndexExpression &expr) {
    return loadFromLValue(expr);
}
CTypedValue CodeGen::visitRValue(const MemberExpression &expr) {
    return loadFromLValue(expr);
}
CTypedValue CodeGen::visitRValue(const SizeOfType &expr) {
    //Need refactoring of the SizeOfType expression
    return CTypedValue::invalid();
    
}
CTypedValue CodeGen::visitRValue(const UnaryExpression &expr) {
    switch(expr.type) {
        case UnaryExpressionType::Indirection: {
            return loadFromLValue(expr);
        }
        case UnaryExpressionType::AdditiveInverse: {
            CTypedValue positive = expr.expression->getRValue(*this);
            if(!positive.isValid()) {
                return CTypedValue::invalid();
            }
            if(!positive.type->isInteger()) {
                //Negating a non-integer
                return CTypedValue::invalid();
            }
            convertToINT(positive);
            positive.value = builder.CreateNeg(
                positive.value,
                "negated"
            );
            return positive; //Now negative
        }
        case UnaryExpressionType::AddressOf: {
            CTypedValue address = expr.expression->getLValue(*this);
            if(!address.isValid()) {
                return CTypedValue::invalid();
            }
            if(!address.type->isFuncNonDesignator()) {
                address.type = address.type->addStar();
            }
            return address;
        }
        case UnaryExpressionType::LogicalInverse: {
            CTypedValue operand = expr.expression->getRValue(*this);
            evaluateCondition(operand, true);
            return operand;
        }
        case UnaryExpressionType::Sizeof: {
            BasicBlock* currentBlock = builder.GetInsertBlock();
            BasicBlock* deadBlock = BasicBlock::Create(
                ctx, 
                "DEAD-BLOCK", 
                currentBlock->getParent()
            );

            //Since computing types is tied with generating code, I need to do this trick
            builder.SetInsertPoint(deadBlock);
            CTypedValue operand = expr.expression->getRValue(*this);
            builder.SetInsertPoint(currentBlock);

            if(!operand.isValid()) {
                return CTypedValue::invalid();
            }

            if(operand.type->isFunc()) {
                //Size of function object not allowed
                return CTypedValue::invalid();
            }

            return CTypedValue(
                ConstantInt::get(
                    IntegerType::getInt32Ty(ctx),
                    M.getDataLayout().getTypeAllocSize(operand.value->getType())
                ),
                BaseCType::get(TypeSpecifier::INT)
            );
        }
        default: {
            throw std::logic_error("Codegen: unrecognized unary expression type");
            return CTypedValue::invalid();
        }
            
    }
}