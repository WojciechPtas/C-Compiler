#include "codegen.h"


AllocaInst* CodeGen::Alloca(Type* type) {
    AllocaInst* ret = allocaBuilder.CreateAlloca(type);
    allocaBuilder.SetInsertPoint(
        allocaBuilder.GetInsertBlock(), 
        allocaBuilder.GetInsertBlock()->begin() 
    );
    return ret;
}

int CodeGen::codeGenTest() {

    // std::cout << filename << std::endl;
    // sys::PrintStackTraceOnErrorSignal(filename);
    // // PrettyStackTraceProgram X(argc, argv);
    // std::shared_ptr<BaseCType> Int = std::make_shared<BaseCType>(TypeSpecifier::INT);

    // //We now generate code for a function definition

    // //We can obtain these directly or indirectly from the AST (current node or children)
    // ParametersInfo funcParams = {{"a", "b"}, {Int, Int}};
    // std::shared_ptr<CType> funcRetType = Int;
    // std::string funcName = "add";
    // //End of information gathering
    
    // CFunctionType funcType(funcRetType, funcParams.types);

    // // FunctionType* funcType = FunctionType::get(
    // //     funcAddRetType->getLLVMType(builder),
    // //     funcAddParams.getLLVMTypes(builder), 
    // //     false
    // // );

    // Function* func = Function::Create(
    //     funcType.getLLVMFuncType(builder), 
    //     GlobalValue::ExternalLinkage,
    //     funcName,
    //     &M
    // );

    // scope.set(
    //     funcName,
    //     CTypedValue(func, std::make_shared<CFunctionType>(funcType))
    // );

    // //If it was just a function declaration we would stop here, but since it is a definition...

    // //We add a new scope onto the stack
    // scope.pushScope();

    // BasicBlock* funcAddBlockEntry = BasicBlock::Create(
    //     ctx,
    //     "add_entry",
    //     func,
    //     NULL //Insert at the end
    // );

    // allocaBuilder.SetInsertPoint(funcAddBlockEntry);
    // builder.SetInsertPoint(funcAddBlockEntry);

    // for(uint i=0; i<funcParams.names.size(); i++) {
    //     Argument* arg = func->arg_begin()+i;
    //     arg->setName(funcParams.names[i]);
    //     AllocaInst *lvalue = Alloca(arg->getType());
    //     builder.CreateStore(arg, lvalue);
    //     CTypedValue typedLvalue(lvalue, funcParams.types[i]);
    //     scope.set(funcParams.names[i], typedLvalue); //Every time we use this we look at the map
    // }

    // Value* LHS = builder.CreateLoad(scope["a"].getLLVMType(builder), scope["a"].value);
    // Value* RHS = builder.CreateLoad(scope["b"].getLLVMType(builder), scope["b"].value);
    // Value* retVal = builder.CreateAdd(LHS, RHS, "retval");
    // builder.CreateRet(retVal);

    // //Just like a delete after a new: don't forget it.
    // scope.popScope();


    // std::error_code EC;
    // raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::OF_Text);

    // verifyFunction(*func, &stream);
    // verifyModule(M, &stream);
    // stream << "\n";

    // M.print(stream, nullptr); /* M is a llvm::Module */


    return 0;
}
