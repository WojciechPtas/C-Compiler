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

const char *Filename = "main.c";

int main(int argc, char **argv) {
  /* If something bad happens, at least print a nice stack trace output */
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  /* Generate LLVM-IR for the c-code:
   *
   *    char *Format;
   *
   *    int printf(const char *, ...);
   *
   *    int main(int argc, char **argv) {
   *      Format = "%s\n";
   *
   *      int i;
   *      i = 1;
   *
   *      while (i < argc) {
   *        printf(Format, argv[i]);
   *        i = i + 1;
   *      }
   *
   *      return 0;
   *    }
   *
   *
   */

  /* Make a global context (only one needed) */
  LLVMContext Ctx;

  /* Create a Module (only one needed) */
  Module M(Filename, Ctx);

  /* Two IR-Builder to output intermediate instructions but also types, ... */
  IRBuilder<> Builder(Ctx), AllocaBuilder(Ctx);

  /* Part 1:
   *
   *    char *Format;
   */

  /* Create some types we need */
  PointerType *PtrType = Builder.getPtrTy();
  Type *CharType = Builder.getInt8Ty();
  Type *IntType = Builder.getInt32Ty();

  /* Create the type of the struct S */
  Type *FormatType = PtrType;


  /* Create the global variable GS */
  GlobalVariable *GlobVarFormatPtr = new GlobalVariable(
          M                                       /* Module & */,
          FormatType                              /* Type * */,
          false                                   /* bool isConstant */,
          GlobalValue::CommonLinkage              /* LinkageType */,
          Constant::getNullValue(FormatType)      /* Constant * Initializer */,
          "Format"                                /* const Twine &Name = "" */,
  /* --------- We do not need this part (=> use defaults) ---------- */
          0                                       /* GlobalVariable *InsertBefore = 0 */,
          GlobalVariable::NotThreadLocal          /* ThreadLocalMode TLMode = NotThreadLocal */,
          0                                       /* unsigned AddressSpace = 0 */,
          false                                   /* bool isExternallyInitialized = false */);

  /* Part 2:
   *
   *    int printf(const char *);
   *
   */
  std::vector<Type *> ArgumentTypes;
  ArgumentTypes.push_back(Builder.getPtrTy());
  FunctionType *FuncPrintfType = FunctionType::get(IntType, ArgumentTypes, /* isVarArg */ true);

  Function *FuncPrintf = Function::Create(
          FuncPrintfType                          /* FunctionType *Ty */,
          GlobalValue::ExternalLinkage            /* LinkageType */,
          "printf"                                /* const Twine &N="" */,
          &M                                      /* Module *M=0 */);

  /* Part 2:
   *
   *    int main(int argc, char **argv) {
   *      Format = "%s\n";
   *
   *      int i;
   *      i = 1;
   *
   *      while (i < argc) {
   *        printf(Format, argv[i]);
   *        i = i + 1;
   *      }
   *
   *      return 0;
   *    }
   *
   */
  Type *FuncMainReturnType = IntType;

  /* Create a vector to store all parameter types */
  std::vector<Type *> FuncMainParamTypes;

  /* Fill the vector with parameter types */
  FuncMainParamTypes.push_back(IntType);
  FuncMainParamTypes.push_back(PtrType);

  /* Create the (function) type of the 'max' function */
  FunctionType *FuncMainType = FunctionType::get(
      FuncMainReturnType, FuncMainParamTypes, /* isVarArg */ false);

  /* Create a Function declaration for 'main' */
  Function *FuncMain = Function::Create(
          FuncMainType                            /* FunctionType *Ty */,
          GlobalValue::ExternalLinkage            /* LinkageType */,
          "main"                                  /* const Twine &N="" */,
          &M                                      /* Module *M=0 */);

  /* Fix the names of the arguments of the function main */
  Function::arg_iterator FuncMainArgIt = FuncMain->arg_begin();
  Argument *ArgArgc = FuncMainArgIt;
  FuncMainArgIt++;
  Argument *ArgArgv = FuncMainArgIt;
  ArgArgc->setName("argc");
  ArgArgv->setName("argv");

  /* Add a first (the entry) basic block to the function. This will
   * automatically convert 'main' from a declaration to a definition.
   */
  BasicBlock *FuncMainEntryBB = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "entry"                                 /* const Twine &Name="" */,
          FuncMain                                /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Set the entry of the function main as the new insert point */
  Builder.SetInsertPoint(FuncMainEntryBB);

  /* The alloca builder always inserts instructions into the entry block.
   * This allows an easier implementation of the 'memory to register' pass and
   * removes the problems caused by an alloca inside a loop. However, this
   * might not be an optimal solution...
   */
  AllocaBuilder.SetInsertPoint(FuncMainEntryBB);


  /* Reset the alloca builder each time before using it
   *
   *   It should not insert any instruction behind the terminator of the entry
   *   block, the easiest way to ensure this is to set it to the beginning of
   *   the entry block each time we insert an alloca. */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgArgcPtr = AllocaBuilder.CreateAlloca(ArgArgc->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgArgc, ArgArgcPtr);

  /* Reset the alloca builder each time before using it */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgArgvPtr = AllocaBuilder.CreateAlloca(ArgArgv->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgArgv, ArgArgvPtr);

  /* Always build string in a 'global' context */
  Value *GlobalStrPtr = Builder.CreateGlobalStringPtr("%s\n");

  /* Store the string in the global variable Format */
  Builder.CreateStore(GlobalStrPtr, GlobVarFormatPtr);

  /* Reset the alloca builder each time before using it */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Allocate stack space for the variable x */
  Value *LocalVarIPtr = AllocaBuilder.CreateAlloca(IntType);

  /* A declaration stores the right value in the location of the left value */
  Builder.CreateStore(Builder.getInt32(1), LocalVarIPtr);

  /* Add a basic block for the header of the IfStmt */
  BasicBlock *WhileHeaderBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-header"                          /* const Twine &Name="" */,
          FuncMain                                 /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Insert a branch from the current basic block to the header of the WhileStmt */
  Builder.CreateBr(WhileHeaderBlock);

  /* Set the header of the IfStmt as the new insert point */
  Builder.SetInsertPoint(WhileHeaderBlock);

  Value *ValI = Builder.CreateLoad(IntType, LocalVarIPtr);
  Value *ValArgc = Builder.CreateLoad(IntType, ArgArgcPtr);
  Value *WhileCondition = Builder.CreateICmpSLT(ValI, ValArgc);

   /* Add a basic block for the consequence of the IfStmt */
  BasicBlock *WhileBodyBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-body"                            /* const Twine &Name="" */,
          FuncMain                                /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Add a basic block for the alternative of the IfStmt */
  BasicBlock *WhileEndBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "while-end"                             /* const Twine &Name="" */,
          FuncMain                                /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Create the conditional branch */
  Builder.CreateCondBr(WhileCondition, WhileBodyBlock, WhileEndBlock);

  /* Start inserting in the while body block */
  Builder.SetInsertPoint(WhileBodyBlock);

  /* Load the value of I */
  Value *ValI2 = Builder.CreateLoad(IntType, LocalVarIPtr);

  /* Adjust the value to i64 for an index computation in an array
   *   The SExtOrTrunc will not change the type if it is already i64 */
  Value *ValI2I64 = Builder.CreateSExtOrTrunc(ValI2, Builder.getInt64Ty());

  /* Create the index vector to access the 'i-th' element
   *   Do not use the trailing 0 as for struct accesses
   *   --> see http://llvm.org/docs/LangRef.html#i-getelementptr */
  std::vector<Value *> ArgvIndices;
  ArgvIndices.push_back(ValI2I64);

  /* Load the base pointer argv */
  Value *ValArgv = Builder.CreateLoad(PtrType, ArgArgvPtr);

  /* Compute the addr of the 'i-th' element of argv */
  Value *ArgvIthElemPtr = Builder.CreateGEP(PtrType, ValArgv, ArgvIndices);

  /* Load the element */
  Value *ArgvIthElem = Builder.CreateLoad(CharType, ArgvIthElemPtr);

  /* Load the value of the global variable Format */
  Value *GlobVarFormatVal = Builder.CreateLoad(PtrType, GlobVarFormatPtr);

  /* Prepare the vector of arguments for the call to printf */
  std::vector<Value *> CallArguments;
  CallArguments.push_back(GlobVarFormatVal);
  CallArguments.push_back(ArgvIthElem);

  /* Create the call */
  Builder.CreateCall(FuncPrintf, CallArguments);

  /* Load the value of x at the beginning of a loop iteration (this instruction
   * is not named, thus it will be given a numerical name if it is dumped) */
  Value *OldIVal = Builder.CreateLoad(IntType, LocalVarIPtr);

  /* Create code for the expression x * n */
  Value *NewIVal = Builder.CreateAdd(OldIVal, Builder.getInt32(1));

  /* Store the new value of x (it is an assignment!) */
  Builder.CreateStore(NewIVal, LocalVarIPtr);

  /* Insert the back loop edge (the branch back to the header) */
  Builder.CreateBr(WhileHeaderBlock);

  /* The while was created, adjust the inserting point to the while end block */
  Builder.SetInsertPoint(WhileEndBlock);

  /* Create the return */
  Builder.CreateRet(Builder.getInt32(0));

  /* Always create a new block after a return statement
   *
   *  This will prevent you from inserting code after a block terminator (here
   *  the return instruction), but it will create a dead basic block instead.
   */
  BasicBlock *ReturnDeadBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "DEAD_BLOCK"                            /* const Twine &Name="" */,
          FuncMain                                /* Function *Parent=0 */,
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

  /* Ensure that we created a 'valid' function */
  verifyFunction(*FuncMain);

  /* Ensure that we created a 'valid' module */
  verifyModule(M);

  /* Dump the final module */
  M.dump();

  return 0;
}
