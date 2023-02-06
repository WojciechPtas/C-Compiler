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

    std::cout << filename << std::endl;
    sys::PrintStackTraceOnErrorSignal(filename);
    // PrettyStackTraceProgram X(argc, argv);

    //We now generate code for a function definition
    //The outerScope is passed somehow
    Scope* outerScope = &global; 

    Scope currentScope(outerScope);

    //Need helper method that works on ParameterList ASTNode and returns this
    BaseCType Int(TypeSpecifier::INT);
    ParametersInfo funcAddParams = {{"a", "b"}, {&Int, &Int}};
    CType *funcAddRetType = &Int;

    FunctionType* funcAddType = FunctionType::get(
        funcAddRetType->getLLVMType(builder),
        funcAddParams.getLLVMTypes(builder), 
        false
    );

    Function* funcAdd = Function::Create(
        funcAddType, 
        GlobalValue::ExternalLinkage,
        "add",
        &M
    );


    BasicBlock* funcAddBlockEntry = BasicBlock::Create(
        ctx,
        "add_entry",
        funcAdd,
        NULL //Insert at the end
    );

    allocaBuilder.SetInsertPoint(funcAddBlockEntry);
    builder.SetInsertPoint(funcAddBlockEntry);

    for(uint i=0; i<funcAddParams.names.size(); i++) {
        Argument* arg = funcAdd->arg_begin()+i;
        arg->setName(funcAddParams.names[i]);
        AllocaInst *lvalue = Alloca(arg->getType());
        builder.CreateStore(arg, lvalue);
        CTypedLValue typedLvalue(lvalue, funcAddParams.types[i]);
        currentScope.set(funcAddParams.names[i], &typedLvalue); //Every time we use this we look at the map
    }

    Value* LHS = builder.CreateLoad(currentScope["a"]->getLLVMType(builder), currentScope["a"]->getValue());
    Value* RHS = builder.CreateLoad(currentScope["b"]->getLLVMType(builder), currentScope["b"]->getValue());
    Value* retVal = builder.CreateAdd(LHS, RHS, "retval");
    builder.CreateRet(retVal);




    std::error_code EC;
    raw_fd_ostream stream(filename, EC, llvm::sys::fs::OpenFlags::OF_Text);

    verifyFunction(*funcAdd, &stream);
    verifyModule(M, &stream);
    stream << "\n";

    M.print(stream, nullptr); /* M is a llvm::Module */


    return 0;
}
