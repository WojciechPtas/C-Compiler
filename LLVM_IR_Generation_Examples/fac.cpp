#include "llvm/IR/Module.h"                /* Module */
#include "llvm/IR/Function.h"              /* Function */
#include "llvm/IR/IRBuilder.h"             /* IRBuilder */
#include "llvm/IR/LLVMContext.h"           /* LLVMContext */
#include "llvm/IR/GlobalValue.h"           /* GlobaleVariable, LinkageTypes */
#include "llvm/IR/Verifier.h"              /* verifyFunction, verifyModule */
#include "llvm/Support/Signals.h"          /* Nice stacktrace output */
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/PrettyStackTrace.h"

using namespace llvm;

const char *Filename = "fac.c";

int main(int argc, char **argv) {

  /* If something bad happens, at least print a nice stacktrace output */
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  /* Generate LLVM-IR for the function:
   *
   *    int fac(int n) {
   *      int x;
   *      x = 1;
   *      while (n) {
   *        x *= n;
   *        n -= 1;
   *      }
   *      return x;
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
   *   int fac(int n) {
   *
   */

  /* Create the return type */
  Type *FuncFacReturnType = Builder.getInt32Ty();

  /* Create a vector to store all parameter types */
  std::vector<Type *> FuncFacParamTypes;

  /* Fill the vector with parameter types */
  FuncFacParamTypes.push_back(Builder.getInt32Ty());

  /* Create the (function) type of the 'fac' function */
  FunctionType *FuncFacType = FunctionType::get(
      FuncFacReturnType, FuncFacParamTypes, /* isVarArg */ false);

  /* Create a function declaration for 'fac' */
  Function *FuncFac = Function::Create(
          FuncFacType                             /* FunctionType *Ty */,
          GlobalValue::ExternalLinkage            /* LinkageType */,
          "fac"                                   /* const Twine &N="" */,
          &M                                      /* Module *M=0 */);

  /* Fix the names of the arguments of the function fac */
  Function::arg_iterator FuncFacArgIt = FuncFac->arg_begin();
  Argument *ArgN = FuncFacArgIt;
  ArgN->setName("n");

  /* Add a first (the entry) basic block to the function. This will
   * automatically convert 'fac' from a declaration to a definition.
   */
  BasicBlock *FuncFacEntryBB = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "entry"                                 /* const Twine &Name="" */,
          FuncFac                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Set a new insert point for the LLVM-IR builder */
  Builder.SetInsertPoint(FuncFacEntryBB);

  /* The alloca builder always inserts instructions into the entry block.
   * This allows an easier implementation of the 'memory to register' pass and
   * removes the problems caused by an alloca inside a loop. However, this
   * might not be an optimal solution...
   */
  AllocaBuilder.SetInsertPoint(FuncFacEntryBB);


  /* Reset the alloca builder each time before using it
   *
   *   It should not insert any instruction behind the terminator of the entry
   *   block, the easiest way to ensure this is to set it to the begining of
   *   the entry block each time we insert an alloca. */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgVarNPtr = AllocaBuilder.CreateAlloca(ArgN->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgN, ArgVarNPtr);


  /* Part 2:
   *
   *   int x;
   *   x = 1;
   *
   */

  /* Reset the alloca builder each time before using it */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Allocate stack space for the variable x */
  Value *LocalVarXPtr = AllocaBuilder.CreateAlloca(Builder.getInt32Ty());

  /* A declaration stores the right value in the location of the left value */
  Builder.CreateStore(Builder.getInt32(1), LocalVarXPtr);


  /* Part 3:
   *
   *   while (n) {
   *     x = x * n;
   *     n = n - 1;
   *   }
   *
   */

  /* Add a basic block for the header of the IfStmt */
  BasicBlock *WhileHeaderBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-header"                          /* const Twine &Name="" */,
          FuncFac                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert a branch from the current basic block to the header of the WhileStmt */
  Builder.CreateBr(WhileHeaderBlock);

  /* Set the header of the IfStmt as the new insert point */
  Builder.SetInsertPoint(WhileHeaderBlock);

  /* A use of n is translated into a load of the current value in the stack
   * location created for n */
  Value *ValN = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarNPtr, "while-cond-n-load");

  /* Create code for the condition (n) <==> (n != 0) */
  Value *WhileCondition =
      Builder.CreateICmpNE(ValN, Builder.getInt32(0), "while-condition");

   /* Add a basic block for the consequence of the IfStmt */
  BasicBlock *WhileBodyBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-body"                            /* const Twine &Name="" */,
          FuncFac                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Add a basic block for the alternative of the IfStmt */
  BasicBlock *WhileEndBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-end"                             /* const Twine &Name="" */,
          FuncFac                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Create the conditional branch */
  Builder.CreateCondBr(WhileCondition, WhileBodyBlock, WhileEndBlock);

  /* Start inserting in the while body block */
  Builder.SetInsertPoint(WhileBodyBlock);

  /* Load the value of x at the beginning of a loop iteration (this instruction
   * is not named, thus it will be given a numerical name if it is dumped) */
  Value *OldXVal = Builder.CreateLoad(Builder.getInt32Ty(), LocalVarXPtr);

  /* Load the value of n at the beginning of a loop iteration */
  Value *OldNVal = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarNPtr);

  /* Create code for the expression x * n */
  Value *NewXVal = Builder.CreateMul(OldXVal, OldNVal);

  /* Store the new value of x (it is an assignment!) */
  Builder.CreateStore(NewXVal, LocalVarXPtr);

  /* Similar, create code for n = n - 1 */
  Value *OldNValReload = Builder.CreateLoad(Builder.getInt32Ty(), ArgVarNPtr);
  Value *NewNVal = Builder.CreateSub(OldNValReload, Builder.getInt32(1));
  Builder.CreateStore(NewNVal, ArgVarNPtr);

  /* Insert the back loop edge (the branch back to the header) */
  Builder.CreateBr(WhileHeaderBlock);

  /* The while was created, adjust the inserting point to the while end block */
  Builder.SetInsertPoint(WhileEndBlock);

  /* Part 4:
   *
   *      return x;
   *
   */

  /* Create a return with the last value of x */
  Value *FinalXVal = Builder.CreateLoad(Builder.getInt32Ty(), LocalVarXPtr);
  Builder.CreateRet(FinalXVal);

  /* Always create a new block after a return statement
   *
   *  This will prevent you from inserting code after a block terminator (here
   *  the return instruction), but it will create a dead basic block instead.
   */
  BasicBlock *ReturnDeadBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "DEAD_BLOCK"                            /* const Twine &Name="" */,
          FuncFac                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert code following the return in this new 'dead' block */
  Builder.SetInsertPoint(ReturnDeadBlock);

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

  /* Ensure that we created a 'valid' module */
  verifyModule(M);

  /* Dump the final module to std::cerr */
  M.dump();

  return 0;
}
