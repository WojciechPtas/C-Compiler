#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/Constant.h"              /* Constant::getNullValue */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

using namespace llvm;

const char *Filename = "max.c";

int main(int argc, char **argv) {
  /* If something bad happens, at least print a nice stack trace output */
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  /* Generate LLVM-IR for the c-fragment:
   *
   *    int max(int a, int b) {
   *      if (a >= b)
   *        return a;
   *      else
   *        return b;
   *    }
   *
   */

  /* Get the global context (only one needed) */
  LLVMContext Ctx;

  /* Create a Module (only one needed) */
  Module M(Filename, Ctx);

  /* Two IR-Builder to output intermediate instructions but also types, ... */
  IRBuilder<> Builder(Ctx), AllocaBuilder(Ctx);

  /* Part 1:
   *
   *    int max(int a, int b) {
   *
   */

  /* Create the return type */
  Type *FuncMaxReturnType = Builder.getInt32Ty();

  /* Create a vector to store all parameter types */
  std::vector<Type *> FuncMaxParamTypes;

  /* Fill the vector with parameter types */
  FuncMaxParamTypes.push_back(Builder.getInt32Ty());
  FuncMaxParamTypes.push_back(Builder.getInt32Ty());

  /* Create the (function) type of the 'max' function */
  FunctionType *FuncMaxType = FunctionType::get(
      FuncMaxReturnType, FuncMaxParamTypes, /* isVarArg */ false);

  /* Create a function declaration for 'max' */
  Function *FuncMax = Function::Create(
          FuncMaxType                             /* FunctionType *Ty */,
          GlobalValue::ExternalLinkage            /* LinkageType */,
          "max"                                   /* const Twine &N="" */,
          &M                                      /* Module *M=0 */);

  /* Add a first (the entry) basic block to the function. This will
   * automatically convert 'max' from a declaration to a definition.
   */
  BasicBlock *FuncMaxEntryBB = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "entry"                                 /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Fix the names of the arguments of the function max */
  Function::arg_iterator FuncMaxArgIt = FuncMax->arg_begin();
  Argument *ArgA = FuncMaxArgIt;
  FuncMaxArgIt++;
  Argument *ArgB = FuncMaxArgIt;
  ArgA->setName("a");
  ArgB->setName("b");

  /* Set a new insert point for the LLVM-IR builder */
  Builder.SetInsertPoint(FuncMaxEntryBB);

  /* The alloca builder always inserts instructions into the entry block.
   * This allows an easier implementation of the 'memory to register' pass and
   * removes the problems caused by an alloca inside a loop. However, this
   * might not be an optimal solution...
   */
  AllocaBuilder.SetInsertPoint(FuncMaxEntryBB);

  /* Reset the alloca builder each time before using it
   *
   *   It should not insert any instruction behind the terminator of the entry
   *   block, the easiest way to ensure this is to set it to the begining of
   *   the entry block each time we insert an alloca. */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgVarAPtr = AllocaBuilder.CreateAlloca(ArgA->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgA, ArgVarAPtr);

  /* Reset the alloca builder each time before using it
   *
   *   It should not insert any instruction behind the terminator of the entry
   *   block, the easiest way to ensure this is to set it to the begining of
   *   the entry block each time we insert an alloca. */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgVarBPtr = AllocaBuilder.CreateAlloca(ArgB->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgB, ArgVarBPtr);

  /* Part 2:
   *
   *   if (a >= b) {
   *     return a;
   *   } else {
   *     return b;
   *   }
   *
   */

  /* Add a basic block for the header of the IfStmt */
  BasicBlock *IfHeaderBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "if-header"                             /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert a branch from the current basic block to the header of the IfStmt */
  Builder.CreateBr(IfHeaderBlock);

  /* Set the header of the IfStmt as the new insert point */
  Builder.SetInsertPoint(IfHeaderBlock);

  /* Create code for the condition (a >= b)
   *  1. Load A and B with the right type */
  Value *ValA = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarAPtr);
  Value *ValB = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarBPtr);

  /*  2. Compare them */
  Value *IfCondition = Builder.CreateICmpSGE(ValA, ValB);

  /* Change the name of the IfStmt condition (after the creation) */
  IfCondition->setName("if-condition");

  /* Add a basic block for the consequence of the IfStmt */
  BasicBlock *IfConsequenceBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "if-consequence"                        /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Add a basic block for the alternative of the IfStmt */
  BasicBlock *IfAlternativeBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "if-alternative"                        /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Add a basic block for the end of the IfStmt (after the if) */
  BasicBlock *IfEndBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "if-end"                                /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Create the conditional branch */
  Builder.CreateCondBr(IfCondition, IfConsequenceBlock, IfAlternativeBlock);

  /* Set the header of the IfConsequenceBlock as the new insert point */
  Builder.SetInsertPoint(IfConsequenceBlock);

  /* Create the 'return a;' statement
   *   1. Load the value of A with the right type */
  Value *ValA2 = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarAPtr);

  /*   2. Create the return */
  Builder.CreateRet(ValA2);

  /* Always create a new block after a return statement
   *
   *  This will prevent you from inserting code after a block terminator (here
   *  the return instruction), but it will create a dead basic block instead.
   */
  BasicBlock *ReturnDeadBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "DEAD_BLOCK"                            /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert code following the return in this new 'dead' block */
  Builder.SetInsertPoint(ReturnDeadBlock);

  /* Insert the jump to the if end block */
  Builder.CreateBr(IfEndBlock);

  /* Set the header of the IfAlternativeBlock as the new insert point */
  Builder.SetInsertPoint(IfAlternativeBlock);

  /* Create the 'return b;' statement
   *   1. Load the value of B with the right type */
  Value *ValB2 = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarBPtr);

  /*   2. Create the return */
  Builder.CreateRet(ValB2);

  /* Always create a new block after a return statement */
  BasicBlock *ReturnDeadBlock2 = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "DEAD_BLOCK"                            /* const Twine &Name="" */,
          FuncMax                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert code following the return in this new 'dead' block */
  Builder.SetInsertPoint(ReturnDeadBlock2);

  /* Insert the jump to the if end block */
  Builder.CreateBr(IfEndBlock);

  /* Continue in the if end block */
  Builder.SetInsertPoint(IfEndBlock);

  /* All code was emitted,.. but the last block might be empty.
   * If the last block does not end with a terminator statement the simple
   * rules created either dead code or the function is a void function without
   * a return on each path. Either way we need to add a terminator instruction
   * to the last block. The idea is to look at the return type of the current
   * function and emit either a void return or a return with the 'NULL' value
   * for this type */
  if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
    Type *CurFuncReturnType = Builder.getCurrentFunctionReturnType();
    if (CurFuncReturnType->isVoidTy()) {
      Builder.CreateRetVoid();
    } else {
      Builder.CreateRet(Constant::getNullValue(CurFuncReturnType));
    }
  }

  /* Ensure that we created a 'valid' function */
  verifyFunction(*FuncMax);

  /* Ensure that we created a 'valid' module */
  verifyModule(M);

  /* Dump the module */
  M.dump();

  return 0;
}
