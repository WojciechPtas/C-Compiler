#include "../../service/CodeGenerator/codegen.h"

#include <memory>
#include "../../model/CType/CStructType.h"
#include "../../model/CType/ConstantZero.h"
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
#include "../expression/BinaryExpressionUtilities.h"
#include "../expression/UnaryExpressionUtilities.h"

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

std::string scalarConditionErrorMsg(const std::string& variable, const std::string& op) {
    std::string str = variable;
    str.append(" is not a scalar condition in a ")
        .append(op)
        .append(" expression");
    return str;
}

std::string illegalOperatorUseErrorMsg(const std::string& op, const std::string& type1, const std::string& type2) {
    std::string str = "Illegal use of ";
    str.append(op)
        .append(" on ")
        .append(type1)
        .append(" and ")
        .append(type2);
    return str;
}

std::string illegalOperatorUseErrorMsg(const std::string& op, const std::string& type) {
    std::string str = "Illegal use of ";
    str.append(op)
        .append(" on ")
        .append(type);
    return str;
}

std::string illegalOperatorUseErrorMsg(const std::string& op) {
    std::string str = "Illegal use of ";
    str.append(op)
        .append(" here");
    return str;
}

std::string noLValueErrorMsg(const std::string& expression) {
    std::string str = expression;
    str.append(" has no lvalue");
    return str;
}

std::string noLValueErrorMsg() {
    return "Expression has no lvalue";
}

std::string incompatibleOperandsErrorMsg(const std::string& expression) {
    std::string str = "Incompatible operands in ";
    str.append(expression);
    return str;
}

void CodeGen::constantZeroToInt(CTypedValue& v1) {
    if(v1.type->isConstantZero()) {
        v1.type = BaseCType::get(TypeSpecifier::INT);
        v1.value = builder.getInt32(0);
    }
}

void CodeGen::constantZeroToVoidPtr(CTypedValue& v1) {
    if(v1.type->isConstantZero()) {
        v1.type = BaseCType::get(TypeSpecifier::VOID, 1);
        v1.value = ConstantPointerNull::get(PointerType::getUnqual(ctx));
    }
}

void CodeGen::matchConstantZeroLeft(CTypedValue& zero, const CType* ctype) {
    if(zero.type->isConstantZero()) {
        if(ctype->isInteger() || !ctype->isPointer()) {
            constantZeroToInt(zero);
        }
        else {//It is a pointer
            constantZeroToVoidPtr(zero);
        }
    }
}

void CodeGen::matchConstantZero(CTypedValue& v1, CTypedValue& v2) {
    matchConstantZeroLeft(v1, v2.type.get());
    matchConstantZeroLeft(v2, v1.type.get());
}

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
            else if(negated) {
                ctv.value = builder.CreateNot(ctv.value, "LogicalNot");
            }
        }
        else {
            //Condition is not scalar! Report an error message on exit
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
            reportError(
                expr.firstTerminal,
                noLValueErrorMsg("LHS of assignment"),
                true
            );
            return CTypedValue::invalid();
        }
        if(!rhs.isValid()) {
            return CTypedValue::invalid();
        }
        //lhs is lvalue, can't come from constant
        matchConstantZeroLeft(rhs, lhs.type.get());

        if(lhs.isConst()) {
            reportError(expr.firstTerminal, "Assignment of constant lvalue");
            return CTypedValue::invalid();
        }

        if(!lhs.type->assignmentCompatible(rhs.type.get())) {
            reportError(
                expr.firstTerminal, 
                incompatibleOperandsErrorMsg(
                    c4::util::expression::stringifyExplicit(expr.type)
                )
            );
            return CTypedValue::invalid();
        }

        if(lhs.type->isInteger()) {
            //Need to unify their sizes
            //It would be nice to issue a warning in this case
            //The following instruction does nothing if they have the same size already
            rhs.value = builder.CreateSExtOrTrunc(
                rhs.value,
                lhs.type->getLLVMType(ctx)
            );
            rhs.type = lhs.type;
        }

        builder.CreateStore(rhs.value, lhs.value);
        return lhs;
    }
    else { //Expression has no lvalue
        // reportError(expr.firstTerminal, noLValueErrorMsg(c4::util::expression::stringifyExplicit(expr.type)));
        return CTypedValue::invalid();
    }
    
}
CTypedValue CodeGen::visitLValue(const CallExpression &expr) {
    //has no lvalue
    // reportError(expr.firstTerminal, noLValueErrorMsg("CallExpression"));
    (void) expr;
    return CTypedValue::invalid();
}

CTypedValue CodeGen::visitConditionalExpression(const ConditionalExpression &expr, bool lvalue) {
    CTypedValue cond = expr.condition->getRValue(*this);
    if(!cond.isValid()) {
        return CTypedValue::invalid();
    }
    constantZeroToInt(cond);
    
    evaluateCondition(cond);
    if(!cond.isValid()) {
        reportError(expr.firstTerminal, 
            scalarConditionErrorMsg("Condition", "Conditional")
        );
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

    builder.CreateCondBr(cond.value, leftEvalBlock, rightEvalBlock);

    builder.SetInsertPoint(leftEvalBlock);
    CTypedValue leftExpr = lvalue ? expr.thenCase->getLValue(*this) : expr.thenCase->getRValue(*this); //May generate control flow! Need to update leftEvalBlock
    leftEvalBlock = builder.GetInsertBlock();
    builder.CreateBr(endBlock);

    builder.SetInsertPoint(rightEvalBlock);
    CTypedValue rightExpr = lvalue ? expr.elseCase->getLValue(*this) : expr.elseCase->getRValue(*this); //May generate control flow! Need to update rightEvalBlock
    rightEvalBlock = builder.GetInsertBlock();
    builder.CreateBr(endBlock);

    if(!(leftExpr.isValid() && rightExpr.isValid())) {
        if(lvalue) {
            reportError(
                expr.firstTerminal,
                noLValueErrorMsg("An operand in Conditional Expression"),
                true
            );
        }
        return CTypedValue::invalid();
    }

    matchConstantZero(leftExpr, rightExpr);

    if(lvalue) {
        if(!leftExpr.type->equivalent(rightExpr.type.get())) {
            reportError(expr.firstTerminal, incompatibleOperandsErrorMsg("Conditional Expression"));
            return CTypedValue::invalid();
        }
    }
    else if(!leftExpr.compatible(rightExpr)) {
        reportError(
            expr.firstTerminal, 
            incompatibleOperandsErrorMsg("Conditional Expression")
        );
        return CTypedValue::invalid();
    }
    else if (!leftExpr.type->equivalent(rightExpr.type.get()) && leftExpr.type->isInteger()) {//Rvalues, integers of different sizes
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
        leftExpr.type = BaseCType::get(TypeSpecifier::INT);
        rightExpr.type = leftExpr.type;
    }


    Type* phiType;
    if(leftExpr.type->isFunc() || rightExpr.type->isFunc()) {
        leftExpr.value = funcToPtr(leftExpr.value);
        rightExpr.value = funcToPtr(rightExpr.value);
        phiType = PointerType::getUnqual(ctx);
    }
    else if (lvalue) {
        phiType = PointerType::getUnqual(ctx);
    }
    else {
        phiType = leftExpr.getLLVMType(ctx);
    }

    //Now they're of the same llvm type
    builder.SetInsertPoint(endBlock);
    PHINode* phi = builder.CreatePHI(
        phiType,
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

CTypedValue CodeGen::visitLValue(const ConditionalExpression &expr) {
    return visitConditionalExpression(expr, true);
}
CTypedValue CodeGen::visitLValue(const ConstantExpression &expr) {
    //has no lvalue
    // reportError(expr.firstTerminal, noLValueErrorMsg("ConstantExpression"));
    (void) expr;
    return CTypedValue::invalid();
}
CTypedValue CodeGen::visitLValue(const IdentifierExpression &expr) {
    if(scope.isVarDeclared(expr.identifier)) {
        return scope[expr.identifier];
    }
    else {
        reportError(expr.firstTerminal, 
            std::string("Undeclared variable ") + expr.identifier);
        return CTypedValue::invalid();
    }
}
CTypedValue CodeGen::visitLValue(const IndexExpression &expr) {
    CTypedValue base = expr.container->getRValue(*this); //RValue of container should be a pointer
    CTypedValue idx = expr.index->getRValue(*this);
    if(!(base.isValid() && idx.isValid())) {
        //You can always obtain the rvalue of an expression. The only way you get invalid is if there was an error beforehand
        return CTypedValue::invalid(); 
    }
    constantZeroToInt(base);
    constantZeroToInt(idx);

    if(!base.type->isPointer() && !base.type->isFunc()) {
        std::swap(base, idx);
    }
    
    if(base.type->isFunc() || base.type->isVoidStar()) {
        reportError(expr.firstTerminal, "Pointer arithmetic on object with no size (IndexExpression)");
        return CTypedValue::invalid();
    }

    if(!base.type->isPointer()) {
        reportError(expr.firstTerminal, "Base of IndexExpression is not a pointer");
        return CTypedValue::invalid();
    }
    if(!idx.type->isInteger()) {
        reportError(expr.firstTerminal, "Index of IndexExpression is not an integer");
        return CTypedValue::invalid();
    }

    pointerAddInt(base, idx);
    base.dereference(); //Case in which it's a function pointer already covered
    //Type of indexedLValue has been dereferenced, but we still want its lvalue
    return base;
}

CTypedValue CodeGen::visitLValue(const MemberExpression &expr) {
    CTypedValue base;
    if(expr.type == MemberExpressionType::Direct) {
        base = expr.container->getLValue(*this);
        if(!base.isValid()) {
            reportError(
                expr.firstTerminal,
                noLValueErrorMsg("Container in Member Expression"),
                true
            );
            return CTypedValue::invalid();
        }
    }
    else {//Pointer. As for dereferencing, we take its R-value
        base = expr.container->getRValue(*this);
        if(!base.isValid()) {
            return CTypedValue::invalid();
        }
        constantZeroToVoidPtr(base);

        if(!base.type->isPointer()) {
            reportError(expr.firstTerminal, "Dereferencing a non-pointer in '->' member access");
            return CTypedValue::invalid();
        }

        if(base.type->isVoidStar()) {
            reportError(expr.firstTerminal, "Dereferencing void pointer");
        }

        base.dereference();
    }

    if(!base.type->isStruct()) {
        reportError(expr.firstTerminal, "Member access of non-struct type");
        return CTypedValue::invalid();
    }

    auto structType = std::dynamic_pointer_cast<const CStructType, const CType>(base.type);
    std::string memberName = expr.member->identifier;
    
    if(!structType->hasMember(memberName)) {
        reportError(expr.firstTerminal, std::string("Accessing nonexistent struct member ") + expr.member->identifier);
        return CTypedValue::invalid();
    }

    uint memberIndex = structType->getIndexOf(memberName);

    Value* value = builder.CreateStructGEP(base.getLLVMType(ctx), base.value, memberIndex);
    std::shared_ptr<const CType> type = structType->getFieldTypes()[memberIndex];
    return CTypedValue(value, type);
    
}
CTypedValue CodeGen::visitLValue(const SizeOfType &expr) {
    //has no lvalue
    // reportError(expr.firstTerminal, noLValueErrorMsg("SizeOfType"));
    (void) expr;
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
        constantZeroToVoidPtr(ptr);
        
        if(!(ptr.type->isPointer() || ptr.type->isFunc())) {
            reportError(expr.firstTerminal, "Dereferencing a non-pointer");
            return CTypedValue::invalid();
        }

        if(ptr.type->isVoidStar()) {
            reportError(expr.firstTerminal, "Dereferencing void pointer");
        }

        if(!ptr.type->isFuncNonDesignator()) {
            ptr.dereference();
        }
        return ptr; //Not a ptr anymore
    }
    //else this kind of expression has no lvalue
    // reportError(expr.firstTerminal, noLValueErrorMsg(c4::util::expression::stringifyExplicit(expr.type)));
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
            if(!lhs.isValid()) {
                return CTypedValue::invalid();
            }
            constantZeroToInt(lhs);
            evaluateCondition(lhs);
            if(!lhs.isValid()) { 
                reportError(expr.firstTerminal, 
                    scalarConditionErrorMsg(
                        "Lhs", 
                        c4::util::expression::stringifyExplicit(expr.type)
                    )
                );
                return CTypedValue::invalid();
            }

            Value* leftCond = lhs.value;
            BasicBlock* leftEvalBlock = builder.GetInsertBlock();
            Function* currentFunction = leftEvalBlock->getParent();

            BasicBlock* lazyEvalBlock = BasicBlock::Create(
                ctx,
                c4::util::expression::stringifyExplicit(expr.type)+"LazyEval",
                currentFunction
            );

            BasicBlock* endBlock = BasicBlock::Create(
                ctx,
                c4::util::expression::stringifyExplicit(expr.type)+"End",
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
            
            CTypedValue rhs = expr.right->getRValue(*this); //May generate control flow! Update lazyEvalBlock
            lazyEvalBlock = builder.GetInsertBlock();
            
            if(!rhs.isValid()) {
                return CTypedValue::invalid();
            }
            constantZeroToInt(rhs);

            evaluateCondition(rhs);
            if(!rhs.isValid()) {
                reportError(expr.firstTerminal, 
                    scalarConditionErrorMsg(
                        "Rhs", 
                        c4::util::expression::stringifyExplicit(expr.type)
                    )
                );
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
            matchConstantZero(lhs, rhs);

            if(!lhs.compatible(rhs)) {
                reportError(
                    expr.firstTerminal, 
                    incompatibleOperandsErrorMsg(
                        c4::util::expression::stringifyExplicit(expr.type)
                    )
                );
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
                reportError(expr.firstTerminal, "Comparisons are not defined over values of this type");
                return CTypedValue::invalid();
            }
            
            unifyIntegerSize(lhs, rhs);
            CTypedValue result;
            result.type = BaseCType::get(TypeSpecifier::BOOL);

            switch(expr.type) {
                case BinaryExpressionType::Equal: {
                    result.value = builder.CreateICmpEQ(
                        lhs.value,
                        rhs.value,
                        "ICmpEQ"
                    );
                    break;
                }
                case BinaryExpressionType::Unequal: {
                    result.value = builder.CreateICmpNE(
                        lhs.value,
                        rhs.value,
                        "ICmpNE"
                    );
                    break;
                }
                case BinaryExpressionType::LessThan: {
                    result.value = builder.CreateICmpSLT(
                        lhs.value,
                        rhs.value,
                        "ICmpSLT"
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

            //Because it's not possible to have voidStar operands, i can safely convert any 0 constant to int
            constantZeroToInt(lhs);
            constantZeroToInt(rhs);

            if(lhs.type->isFunc() || rhs.type->isFunc() || lhs.type->isVoidStar() || rhs.type->isVoidStar()) {
                reportError(expr.firstTerminal, "Arithmetic over pointers to objects without size (e.g. functions) is not allowed");
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
                    reportError(
                        expr.firstTerminal, 
                        illegalOperatorUseErrorMsg(
                            c4::util::expression::stringifyExplicit(expr.type),
                            "Pointer",
                            "Integer"
                        )
                    );
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
                    reportError(
                        expr.firstTerminal, 
                        illegalOperatorUseErrorMsg(
                            c4::util::expression::stringifyExplicit(expr.type),
                            "Integer",
                            "Pointer"
                        )
                    );
                    return CTypedValue::invalid();
                }
                else if(expr.type == BinaryExpressionType::Subtraction) {
                    reportError(
                        expr.firstTerminal, 
                        illegalOperatorUseErrorMsg(
                            c4::util::expression::stringifyExplicit(expr.type),
                            "Integer",
                            "Pointer"
                        )
                    );
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
                    reportError(
                        expr.firstTerminal, 
                        illegalOperatorUseErrorMsg(
                            c4::util::expression::stringifyExplicit(expr.type),
                            "Pointer",
                            "Pointer"
                        )
                    );
                    return CTypedValue::invalid();
                }
            }
            else {
                reportError(
                        expr.firstTerminal, 
                        illegalOperatorUseErrorMsg(
                            c4::util::expression::stringifyExplicit(expr.type)
                        )
                    );
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
        reportError(expr.firstTerminal, "Calling a non-function object");
        return CTypedValue::invalid(); 
    }
    auto funcType = std::dynamic_pointer_cast<const CFunctionType, const CType>(called.type);
    if(expr.arguments.arguments.size() != funcType->paramTypes.size()) {
        reportError(expr.firstTerminal, "Incorrect number of parameters in function call");        
        return CTypedValue::invalid();
    }

    std::vector<Value*> llvmArguments;

    for(uint i=0; i<funcType->paramTypes.size(); i++) {
        //I take the Rvalue of one of the arguments in the expression
        CTypedValue argRvalue = expr.arguments.arguments[i]->getRValue(*this);
        if(!argRvalue.isValid()) {
            return CTypedValue::invalid();
        }
        matchConstantZeroLeft(argRvalue, funcType->paramTypes[i].get());
        //I check if they have compatible types
        if(!funcType->paramTypes[i]->assignmentCompatible(argRvalue.type.get())) {
            reportError(expr.firstTerminal, 
                std::string("Incompatible parameters at position ")
                    .append(std::to_string(i))
                    .append(" of function call (starting from 0)"));
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
    return visitConditionalExpression(expr, false);
}

CTypedValue CodeGen::visitRValue(const ConstantExpression &expr) {
    switch(expr.type) {
        case ConstantType::Character: {
            std::string constValue = expr.value;
            c4::util::lexer::parseEscapeSequences(constValue);
            return CTypedValue(
                builder.getInt8(constValue[0]),
                BaseCType::get(TypeSpecifier::CHAR)
            );
        }
        case ConstantType::Decimal: {
            try {
                int val = std::stoi(expr.value);
                CTypedValue result;
                result.value = builder.getInt32(val);

                if(val == 0) {
                    result.type = ConstantZero::get();
                }
                else {
                    result.type = BaseCType::get(TypeSpecifier::INT);
                }
                return result;

            } catch(std::out_of_range&) {
                reportError(expr.firstTerminal, "Constant outside range of representable values");
                return CTypedValue::invalid();
            }
            
        }
        case ConstantType::String: {
            std::string constValue = expr.value;
            c4::util::lexer::parseEscapeSequences(constValue);
            return CTypedValue(
                builder.CreateGlobalStringPtr(constValue),
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
    //Nothing can go wrong here if good error detection was done at AST construction
    shared_ptr<const CType> ctype = getCtype(expr.type);
    
    if(!ctype->isComplete()) {
        reportError(
            expr.firstTerminal,
            illegalOperatorUseErrorMsg(
                "sizeof(type-name)",
                "Incomplete object"
            )
        );
        return CTypedValue::invalid();
    }

    Type* type;
    if(ctype->isFunc()) {
        type = PointerType::getUnqual(ctx);
    }
    else {
        type = ctype->getLLVMType(ctx);
    }
    TypeSize size = M.getDataLayout().getTypeAllocSize(type);
    
    return CTypedValue(
        ConstantInt::get(
            IntegerType::getInt32Ty(ctx),
            size
        ),
        BaseCType::get(TypeSpecifier::INT)
    );
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
            constantZeroToInt(positive);
            
            if(!positive.type->isInteger()) {
                reportError(
                    expr.firstTerminal,
                    illegalOperatorUseErrorMsg(c4::util::expression::stringifyExplicit(expr.type))
                );
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
                reportError(
                    expr.firstTerminal,
                    noLValueErrorMsg("Operand of Address Of"),
                    true
                );
                return CTypedValue::invalid();
            }
            if(!address.type->isFuncNonDesignator()) {
                address.type = address.type->addStar();
            }
            return address;
        }
        case UnaryExpressionType::LogicalInverse: {
            CTypedValue operand = expr.expression->getRValue(*this);
            if(!operand.isValid()) {
                return CTypedValue::invalid();
            }
            constantZeroToInt(operand);

            evaluateCondition(operand, true);
            if(!operand.isValid()) {
                reportError(expr.firstTerminal, 
                    scalarConditionErrorMsg("Operand", c4::util::expression::stringifyExplicit(expr.type))
                );
            }
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
            builder.CreateUnreachable();
            builder.SetInsertPoint(currentBlock);

            if(!operand.isValid()) {
                return CTypedValue::invalid();
            }
            constantZeroToInt(operand);

            if(!operand.type->isComplete()) {
                reportError(
                    expr.firstTerminal,
                    illegalOperatorUseErrorMsg(
                        "sizeof(expr)",
                        "Incomplete object"
                    )
                );
                return CTypedValue::invalid();
            }
            
            Type* type;
            if(operand.type->isFunc()) {
                type = PointerType::getUnqual(ctx);
            }
            else {
                type = operand.type->getLLVMType(ctx);
            }
            TypeSize size = M.getDataLayout().getTypeAllocSize(type);
            // deadBlock->eraseFromParent(); cannot do it because of other possible users!

            return CTypedValue(
                ConstantInt::get(
                    IntegerType::getInt32Ty(ctx),
                    size
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
