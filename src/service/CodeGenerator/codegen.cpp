#include "codegen.h"
using namespace c4::model::ctype;
using namespace c4::model::expression;
using namespace llvm;

AllocaInst* CodeGen::Alloca(Type* type, const std::string& name) {
    AllocaInst* ret = allocaBuilder.CreateAlloca(type, nullptr, name);
    allocaBuilder.SetInsertPoint(
        allocaBuilder.GetInsertBlock(), 
        allocaBuilder.GetInsertBlock()->begin() 
    );
    return ret;
}

AllocaInst* CodeGen::Alloca(const CType* type, const std::string& name) {
    return Alloca(type->getLLVMType(ctx), name);
}

AllocaInst* CodeGen::Alloca(std::shared_ptr<const CType> type, const std::string& name) {
    return Alloca(type->getLLVMType(ctx), name);
}

GlobalVariable* CodeGen::GlobalAlloca(Type* type, const std::string& name) {
    return new GlobalVariable(
        M                                 /* Module & */,
        type                              /* Type * */,
        false                             /* bool isConstant */,
        GlobalValue::CommonLinkage        /* LinkageType */,
        Constant::getNullValue(type)      /* Constant * Initializer */,
        name                              /* const Twine &Name = "" */
    );
}

GlobalVariable* CodeGen::GlobalAlloca(const CType* type, const std::string& name) {
    return GlobalAlloca(type->getLLVMType(ctx), name);
}

GlobalVariable* CodeGen::GlobalAlloca(std::shared_ptr<const CType> type, const std::string& name) {
    return GlobalAlloca(type->getLLVMType(ctx), name);
}

int CodeGen::codeGenTest() {
    


    std::cout << filename << std::endl;
    sys::PrintStackTraceOnErrorSignal(filename);
    // PrettyStackTraceProgram X(argc, argv);
    std::shared_ptr<const BaseCType> Int = std::make_shared<const BaseCType>(TypeSpecifier::INT);

    //We now generate code for a function definition

    //We can obtain these directly or indirectly from the AST (current node or children)
    ParametersInfo funcParams = {{"a", "b"}, {Int, Int}};
    std::shared_ptr<const CType> funcRetType = Int;
    std::string funcName = "add";
    //End of information gathering
    
    CFunctionType funcType(funcRetType, funcParams.types);

    // FunctionType* funcType = FunctionType::get(
    //     funcAddRetType->getLLVMType(builder),
    //     funcAddParams.getLLVMTypes(builder), 
    //     false
    // );

    Function* func = Function::Create(
        funcType.getLLVMFuncType(ctx), 
        GlobalValue::ExternalLinkage,
        funcName,
        &M
    );

    scope.declareVar(
        funcName,
        CTypedValue(func, std::make_shared<CFunctionType>(funcType))
    );

    //If it was just a function declaration we would stop here, but since it is a definition...

    //We add a new scope onto the stack
    scope.pushScope();

    BasicBlock* funcAddBlockEntry = BasicBlock::Create(
        ctx,
        "add_entry",
        func,
        NULL //Insert at the end
    );

    // int merd(int, int);
    // int (*merd2)(int, int);
    // // (merd+1)(3,4); works!
    // int *p;
    // *(merd2+2);
    // 2+merd2;
    // // (merd2+1) = merd2;
    // 2+p;
    // int (**cazzo)(int, int);
    // merd2(1, 1);
    // (*merd2)(1,1);

    allocaBuilder.SetInsertPoint(funcAddBlockEntry);
    builder.SetInsertPoint(funcAddBlockEntry);

    for(uint i=0; i<funcParams.names.size(); i++) {
        Argument* arg = func->arg_begin()+i;
        arg->setName(funcParams.names[i]);
        AllocaInst *lvalue = Alloca(arg->getType(), arg->getName().str());
        builder.CreateStore(arg, lvalue);
        CTypedValue typedLvalue(lvalue, funcParams.types[i]);
        scope.declareVar(funcParams.names[i], typedLvalue); //Every time we use this we look at the map
    }

    Value* LHS = builder.CreateLoad(scope["a"].getLLVMType(ctx), scope["a"].value);
    Value* RHS = builder.CreateLoad(scope["b"].getLLVMType(ctx), scope["b"].value);
    Value* retVal = builder.CreateAdd(LHS, RHS, "retval");
    builder.CreateRet(retVal);
    
    //Just like a delete after a new: don't forget it.
    scope.popScope();


    std::error_code EC;
    raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::OF_Text);

    verifyFunction(*func, &stream);
    verifyModule(M, &stream);
    stream << "\n";

    M.print(stream, nullptr); /* M is a llvm::Module */


    return 0;
}
