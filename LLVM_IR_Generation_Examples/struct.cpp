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

const char *Filename = "struct.c";

int main(int argc, char **argv) {

  /* If something bad happens, at least print a nice stack trace output */
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  /* Generate LLVM-IR for the c-fragment:
   *
   *    struct S {
   *      int a;
   *      int b;
   *      int c;
   *      struct S *d;
   *    } GS;
   *
   *    int f(struct S *AS) {
   *      struct S LS;
   *      return GS.a + AS->b + LS.c + LS.d->c;
   *    }
   *
   */

  /* Get the global context (only one needed) */
  LLVMContext Ctx;

  /* Create a Module (only one needed) */
  Module M(Filename, Ctx);

  /* An IR-Builder to output intermediate instructions but also types, ... */
  IRBuilder<> Builder(Ctx), AllocaBuilder(Ctx);


  /* Part 1:
   *
   *    struct S {
   *      int a;
   *      int b;
   *      int c;
   *      struct S *d;
   *    } GS;
   *
   */

  /* Create the type of the struct S */
  StructType *StructSType = StructType::create(Ctx, /* Name */ "struct.S");

  /* Create a vector of types for the types of the struct members and fill it */
  std::vector<Type *> StructMemberTypes;
  StructMemberTypes.push_back(Builder.getInt32Ty());
  StructMemberTypes.push_back(Builder.getInt32Ty());
  StructMemberTypes.push_back(Builder.getInt32Ty());
  StructMemberTypes.push_back(PointerType::getUnqual(StructSType));

  /* Populate the struct type with the members */
  StructSType->setBody(StructMemberTypes);

  /* Create the global variable GS */
  GlobalVariable *GlobVarGSPtr = new GlobalVariable(
          M                                       /* Module & */,
          StructSType                             /* Type * */,
          false                                   /* bool isConstant */,
          GlobalValue::CommonLinkage              /* LinkageType */,
          Constant::getNullValue(StructSType)     /* Constant * Initializer */,
          "GS"                                    /* const Twine &Name = "" */,
  /* --------- We do not need this part (=> use defaults) ---------- */
          0                                       /* GlobalVariable *InsertBefore = 0 */,
          GlobalVariable::NotThreadLocal          /* ThreadLocalMode TLMode = NotThreadLocal */,
          0                                       /* unsigned AddressSpace = 0 */,
          false                                   /* bool isExternallyInitialized = false */);

  /* Part 2:
   *
   *    int f(struct S *AS) {
   *      struct S LS;
   *      return GS.a + AS->b + LS.c + LS.d->c;
   *    }
   *
   */

  /* Create the return type */
  Type *FuncFReturnType = Builder.getInt32Ty();

  /* Create a vector to store all parameter types */
  std::vector<Type *> FuncFParamTypes;

  /* Fill the vector with parameter types */
  FuncFParamTypes.push_back(PointerType::getUnqual(StructSType));

  /* Create the (function) type of the 'f' function */
  FunctionType *FuncFType = FunctionType::get(
      FuncFReturnType, FuncFParamTypes, /* isVarArg */ false);

  /* Create a function declaration for 'f' */
  Function *FuncF = Function::Create(
          FuncFType                               /* FunctionType *Ty */,
          GlobalValue::ExternalLinkage            /* LinkageType */,
          "f"                                     /* const Twine &N="" */,
          &M                                      /* Module *M=0 */);

  /* Fix the names of the arguments of the function f */
  Function::arg_iterator FuncFArgIt = FuncF->arg_begin();
  Argument *ArgAS = FuncFArgIt;
  ArgAS->setName("AS");

  /* Add a first (the entry) basic block to the function. This will
   * automatically convert 'f' from a declaration to a definition.
   */
  BasicBlock *FuncFEntryBB = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "entry"                                 /* const Twine &Name="" */,
          FuncF                                   /* Function *Parent=0 */,
          0                                       /* BasicBlock *InsertBefore=0 */);

  /* Set a new insert point for the LLVM-IR builder */
  Builder.SetInsertPoint(FuncFEntryBB);

  /* The alloca builder always inserts instructions into the entry block.
   * This allows an easier implementation of the 'memory to register' pass and
   * removes the problems caused by an alloca inside a loop. However, this
   * might not be an optimal solution...
   */
  AllocaBuilder.SetInsertPoint(FuncFEntryBB);


  /* Reset the alloca builder each time before using it
   *
   *   It should not insert any instruction behind the terminator of the entry
   *   block, the easiest way to ensure this is to set it to the beginning of
   *   the entry block each time we insert an alloca. */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Store each argument on the stack to allow easy (phi-less) modifications.
   *   1. Allocate a stack slot */
  Value *ArgVarASPtrPtr = AllocaBuilder.CreateAlloca(ArgAS->getType());

  /*   2. Store the argument value */
  Builder.CreateStore(ArgAS, ArgVarASPtrPtr);

  /* Reset the alloca builder each time before using it */
  AllocaBuilder.SetInsertPoint(AllocaBuilder.GetInsertBlock(),
                               AllocaBuilder.GetInsertBlock()->begin());

  /* Allocate the space for the local variable LS */
  Value *LocalVarLSPtr = AllocaBuilder.CreateAlloca(
      StructSType,                               /* Type * */
      Builder.getInt32(1),                       /* Value * ArraySize */
      "LS"                                       /* Name */);


  /* Access the element 'a' of 'GS'
   *   1. Compute the offset of 'a' in 'GS' by its index */
  std::vector<Value *> ElementIndexes;

  /* Always use a 0 as first index */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Then the index of the element in question */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Use a getElementPointer (GEP) instruction to compute the offset from the
   * base pointer (here the GS pointer) */
  Value *VarGSAPtr = Builder.CreateInBoundsGEP(StructSType, GlobVarGSPtr, ElementIndexes);

  /* Load the value of the element */
  LoadInst *ValGSA = Builder.CreateLoad(Builder.getInt32Ty(), VarGSAPtr);


  /* Access the element 'b' of 'AS'
   *   0. Load the element 'AS' from the stack (it was an -> access !) */
  LoadInst *LocalVarASPtr = Builder.CreateLoad(Builder.getPtrTy(), ArgVarASPtrPtr);

  /*   1. Compute the offset of 'b' in 'AS' by its index */
  ElementIndexes.clear();

  /* Always use a 0 as first index to access struct elements */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Then the index of the element in question */
  ElementIndexes.push_back(Builder.getInt32(1));

  /* Use a getElementPointer (GEP) instruction to compute the offset from the
   * base pointer (here the GS pointer) */
  Value *VarASBPtr = Builder.CreateInBoundsGEP(StructSType, LocalVarASPtr, ElementIndexes);

  /* Load the value of the element */
  LoadInst *ValASB = Builder.CreateLoad(Builder.getInt32Ty(), VarASBPtr);

  /* Add the two values computed so far */
  Value *Sum0 = Builder.CreateAdd(ValGSA, ValASB);


  /* Access the element 'c' of 'LS'
   *   1. Compute the offset of 'c' in 'LS' by its index */
  ElementIndexes.clear();

  /* Always use a 0 as first index to access struct elements */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Then the index of the element in question */
  ElementIndexes.push_back(Builder.getInt32(2));

  /* Use a getElementPointer (GEP) instruction to compute the offset from the
   * base pointer (here the GS pointer) */
  Value *VarLSCPtr = Builder.CreateInBoundsGEP(StructSType, LocalVarLSPtr, ElementIndexes);

  /* Load the value of the element */
  LoadInst *ValLSC = Builder.CreateLoad(Builder.getInt32Ty(), VarLSCPtr);

  /* Add the first sum and the last value */
  Value *Sum1 = Builder.CreateAdd(Sum0, ValLSC);


  /* Access the element 'd->c' of 'LS'
   *   1. Compute the offset of 'd' in 'LS' by its index */
  ElementIndexes.clear();

  /* Always use a 0 as first index to access struct elements */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Then the index of the element in question */
  ElementIndexes.push_back(Builder.getInt32(3));

  /* Use a getElementPointer (GEP) instruction to compute the offset from the
   * base pointer (here the GS pointer) */
  Value *VarLSDPtr = Builder.CreateInBoundsGEP(StructSType, LocalVarLSPtr, ElementIndexes);

  /* Load the value of the element */
  LoadInst *ValLSD = Builder.CreateLoad(Builder.getPtrTy(), VarLSDPtr);

  /*   1. Compute the offset of 'd' in 'LS.c' by its index */
  ElementIndexes.clear();

  /* Always use a 0 as first index to access struct elements */
  ElementIndexes.push_back(Builder.getInt32(0));

  /* Then the index of the element in question */
  ElementIndexes.push_back(Builder.getInt32(2));

  /* Use a getElementPointer (GEP) instruction to compute the offset from the
   * base pointer (here the GS pointer) */
  Value *VarLSDCPtr = Builder.CreateInBoundsGEP(StructSType, ValLSD, ElementIndexes);

  /* Load the value of the element */
  LoadInst *ValLSDC = Builder.CreateLoad(Builder.getInt32Ty(), VarLSDCPtr);

  /* Add the first sum and the last value */
  Value *Sum2 = Builder.CreateAdd(Sum1, ValLSDC);

  /* Return the sum */
  Builder.CreateRet(Sum2);

  /* Always create a new block after a return statement
   *
   *  This will prevent you from inserting code after a block terminator (here
   *  the return instruction), but it will create a dead basic block instead.
   */
  BasicBlock *ReturnDeadBlock = BasicBlock::Create(
          Ctx                                     /* LLVMContext &Context */,
          "DEAD_BLOCK"                            /* const Twine &Name="" */,
          FuncF                                   /* Function *Parent=0 */,
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
