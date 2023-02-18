#include "../../service/CodeGenerator/codegen.h"

using namespace c4::model::statement;
using namespace c4::model::token;
using namespace c4::model::declaration;
using namespace c4::model::ctype;
using namespace llvm;
// DONE
void CodeGen::visit(const c4::model::statement::CompoundStatement& s){
    if(SecondPhase) return;
    SecondPhase= true;
    scope.pushScope();
    for(auto& a : s.block_of_statements){
        a->accept(*this);
    }
    SecondPhase=false;
     for(auto& a : s.block_of_statements){
        a->accept(*this);
    }
    scope.popScope();
    return;
}
// CODEGEN EXPR
void CodeGen::visit(const c4::model::statement::ExpressionStatement& s){
    if(FirstPhase||SecondPhase) return;
    if(s.expr==nullptr) return;
    s.expr->getRValue(*this);
}
// CODEGEN EXPR
void CodeGen::visit(const c4::model::statement::IterationStatement& s){
    if(FirstPhase||SecondPhase){
    if(s.statement!=nullptr){
        s.statement->accept(*this);
        }
        return;
    }
    else{
        BasicBlock* whileHeader = BasicBlock::Create(
            ctx,
            "whileHeader",
            builder.GetInsertBlock()->getParent(),
            NULL //Insert at the end
        );
        BasicBlock* whileBody = BasicBlock::Create(
            ctx,
            "whileBody",
            builder.GetInsertBlock()->getParent(),
            NULL
        );
        BasicBlock* afterWhile = BasicBlock::Create(
            ctx,
            "afterWhile",
            builder.GetInsertBlock()->getParent(),
            NULL
        );
        
        headerBlocks.push(whileHeader);
        afterBlocks.push(afterWhile);
        builder.CreateBr(whileHeader);
        builder.SetInsertPoint(whileHeader);
       
        CTypedValue cond = s.expr->getRValue(*this);
        if(!cond.isValid()){
            return;
        }
        evaluateCondition(cond,false);
        if(!cond.isValid()) {
            reportError(s.firstTerminal,"Not scalar condition.");
            //err
        }
        builder.CreateCondBr(cond.value,whileBody,afterWhile);
        builder.SetInsertPoint(whileBody);
        // CODEGEN BODY
        if(s.statement!=nullptr) s.statement->accept(*this);
        builder.CreateBr(whileHeader);
        builder.SetInsertPoint(afterWhile);
        headerBlocks.pop();
        afterBlocks.pop();
        return;
    }
}
void CodeGen::visit(const c4::model::statement::JumpStatement& s){
    if(FirstPhase||SecondPhase) return;
    if(s.k==kind::_goto){
        if(gotoLabels.find(s.gotoIdentifier)!=gotoLabels.end()){
            builder.CreateBr(gotoLabels[s.gotoIdentifier]);
        BasicBlock* deadBlock = BasicBlock::Create(
            ctx,
            "dead-block",
            builder.GetInsertBlock()->getParent(),
            NULL //Insert at the end
        );
        builder.SetInsertPoint(deadBlock);
        }
        else{
            std::string msg = "Label with this name: " + s.gotoIdentifier + " was not declared.";
            reportError(s.firstTerminal,msg);
        }
    }
    else if(s.k==kind::_return){ // return;
        auto ret = builder.GetInsertBlock()->getParent();
        auto ret_type = ret->getReturnType();
        if(s.returnExpression!=nullptr){
            if(ret_type->isVoidTy()){
                reportError(s.firstTerminal,"Return statement with expression in void type function");
            }
            else{
                CTypedValue val = s.returnExpression->getRValue(*this);
                if(currentFunc->retType->compatible(val.type.get())){
                    builder.CreateRet(val.value);
                    BasicBlock* deadBlock = BasicBlock::Create(
                        ctx,
                        "dead-block",
                        builder.GetInsertBlock()->getParent(),
                        NULL //Insert at the end
                        );
                    builder.SetInsertPoint(deadBlock);
                }
                else{
                    reportError(s.firstTerminal,"Return statement with wrong type expression");

                    // error
                }
            }
        }
        else{
            if(ret_type->isVoidTy()){
                builder.CreateRetVoid();
                BasicBlock* deadBlock = BasicBlock::Create(
                        ctx,
                        "dead-block",
                        builder.GetInsertBlock()->getParent(),
                        NULL //Insert at the end
                        );
                builder.SetInsertPoint(deadBlock);
            }
            else{
                reportError(s.firstTerminal, "Empty return statement in non-void function");
            }
        }
    }
    else if(s.k==kind::_break){
        if(afterBlocks.size()!=0){
            builder.CreateBr(afterBlocks.top());
            BasicBlock* deadBlock = BasicBlock::Create(
                        ctx,
                        "dead-block",
                        builder.GetInsertBlock()->getParent(),
                        NULL //Insert at the end
                        );
                    builder.SetInsertPoint(deadBlock);
            return;
        }
        else{
            reportError(s.firstTerminal,"Break outside the loop");
        }
    }
    else{ // _continue
        if(headerBlocks.size()!=0){
            builder.CreateBr(headerBlocks.top());
            BasicBlock* deadBlock = BasicBlock::Create(
                        ctx,
                        "dead-block",
                        builder.GetInsertBlock()->getParent(),
                        NULL //Insert at the end
                        );
            builder.SetInsertPoint(deadBlock);
            return;
        }
        else{
            reportError(s.firstTerminal,"Continue outside the loop.");
        }
    }
}
void CodeGen::visit(const c4::model::statement::LabeledStatement& s){
    if(SecondPhase) {
        s.statement->accept(*this);
        return;
    }

    if(FirstPhase){
        BasicBlock* jumpBlock = BasicBlock::Create(
            ctx,
            s.identifier,
            /*builder.GetInsertBlock()->getParent(),*/
            NULL //Insert at the end
        );
        if(gotoLabels.find(s.identifier)!=gotoLabels.end()){
            std::string msg= "Label with name: "+s.identifier+" was already declared.";
            reportError(s.firstTerminal, msg);
        }
        else{
            gotoLabels[s.identifier]=jumpBlock;
        }
    }
    else{
        gotoLabels[s.identifier]->insertInto(builder.GetInsertBlock()->getParent());
        builder.CreateBr(gotoLabels[s.identifier]);
        builder.SetInsertPoint(gotoLabels[s.identifier]);
    }
    s.statement->accept(*this);
}
void CodeGen::visit(const c4::model::statement::SelectionStatement& s){
    if(FirstPhase||SecondPhase){
        if(s.thenStatement!=nullptr) s.thenStatement->accept(*this);
        if(s.elseStatement!=nullptr) s.elseStatement->accept(*this);
    }
    else{
        BasicBlock* ifHeader = BasicBlock::Create(
            ctx,
            "if-header",
            builder.GetInsertBlock()->getParent(),
            NULL //Insert at the end
        );
        BasicBlock* thenBlock = BasicBlock::Create(
            ctx,
            "than-block",
            builder.GetInsertBlock()->getParent(),
            NULL //Insert at the end
        );
        BasicBlock* elseBlock = BasicBlock::Create(
            ctx,
            "else-block",
            /*builder.GetInsertBlock()->getParent(),*/
            NULL //Insert at the end
        );
        BasicBlock* endBlock = BasicBlock::Create(
            ctx,
            "if-end",
            /*builder.GetInsertBlock()->getParent(),*/
            NULL //Insert at the end
        );
        builder.CreateBr(ifHeader);
        builder.SetInsertPoint(ifHeader);
       
        CTypedValue cond = s.ifExpr->getRValue(*this);
        if(!cond.isValid()){
            return;
        }
        evaluateCondition(cond,false);
        if(!cond.isValid()) {
            reportError(s.firstTerminal,"Not scalar condition.");
            //err
        }
        if(s.elseStatement!=nullptr){
            builder.CreateCondBr(cond.value,thenBlock,elseBlock);
        }
        else{
            builder.CreateCondBr(cond.value,thenBlock,endBlock);
        }
        builder.SetInsertPoint(thenBlock);
        if(s.thenStatement!=nullptr) s.thenStatement->accept(*this);
        builder.CreateBr(endBlock);
        if(s.elseStatement!=nullptr){
            elseBlock->insertInto(builder.GetInsertBlock()->getParent());
            builder.SetInsertPoint(elseBlock);
            s.elseStatement->accept(*this);
            builder.CreateBr(endBlock);
        }
        endBlock->insertInto(builder.GetInsertBlock()->getParent());
        builder.SetInsertPoint(endBlock);
    }
}

void CodeGen::visit(const c4::model::declaration::DeclarationSpecifier & s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::Declarator& s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::DirectDeclarator& s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::DirectDeclarator2& s){
    auto a =s; // WE DO NOTHING
    return; 
}


struct Var{
    std::string name;
    std::string structname="";
    std::shared_ptr<const CType> type;
    std::shared_ptr<ParametersInfo> params;
};
Var buildParam(std::shared_ptr<ParameterDeclaration> param);
ParametersInfo buildParameters(std::shared_ptr<ParameterTypeList> params);
Var buildVar(std::shared_ptr<DirectDeclarator> p, Var returnVal);
Var buildVar(std::shared_ptr<Pointer> p, Var returnVal);
Var buildVar(std::shared_ptr<Declarator> d, Var returnVal);
Var buildFromDS(std::shared_ptr<DeclarationSpecifier> ds){
    Var a;
    a.name="";
    switch(ds->keyword){
            case Keyword::Int:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::INT);
            break;
            case Keyword::Char:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::CHAR);
            break;
            case Keyword::Void:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::VOID);
            break;
            default:
            a.type= nullptr;
            break;
    }
    return a;
}
Var buildfromDeclaration(std::shared_ptr<Declaration> d){
    Var a = buildFromDS(d->ds);
    a = buildVar(d->declarator,a);
    return a;
}
ParametersInfo buildStruct(std::shared_ptr<StructDeclarationList> s){
    Var field;
    std::vector<std::string> names;
    std::vector<std::shared_ptr<const CType>> fields;
    for(auto& a : s->declarations){
        field = buildfromDeclaration(a);
        names.push_back(field.name);
        fields.push_back(field.type);
    }
    ParametersInfo p;
    p.names=names;
    p.types=fields;
    return p;
}
Var buildStruct(std::shared_ptr<StructUnionSpecifier> s){
    Var a;
    a.structname=s->name;
    if(s->declarations!=nullptr){    
        auto p = buildStruct(s->declarations);
        a.type=std::make_shared<const CStructType>(p.names,p.types);
    }
    else{
        std::vector<std::string> names;
        std::vector<std::shared_ptr<const CType>> types;
        a.type=std::make_shared<const CStructType>(names,types);
    }
    return a;
}
Var buildDeclarationFromDS(std::shared_ptr<DeclarationSpecifier> ds){
    Var a;
    a.name="";
    switch(ds->keyword){
            case Keyword::Int:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::INT);
            break;
            case Keyword::Char:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::CHAR);
            break;
            case Keyword::Void:
            a.type= std::make_shared<const BaseCType>(TypeSpecifier::VOID);
            break;
            case Keyword::Struct:
            a=buildStruct(ds->structorunion);
            break;
            default:
            a.type= nullptr;
            break;
    }
    return a;
}


Var buildParam(std::shared_ptr<ParameterDeclaration> param){
    auto base = buildFromDS(param->type);
    return param->dec == nullptr ? base :   buildVar(param->dec,base);
    // std::cout << "param built\n";
}
// DONE!
ParametersInfo buildParameters(std::shared_ptr<ParameterTypeList> params){
    // std::cout<<"Build params\n";
    std::vector<std::shared_ptr<const CType>> vec;
    std::vector<std::string> names;
    for(const auto& a : params->params){
        auto variable = buildParam(a);
        vec.push_back(variable.type);
        names.push_back(variable.name);
    }
    ParametersInfo info;
    info.names=names;
    info.types=vec;
    return info;
}
// DONE!
bool isEmpty(std::shared_ptr<DirectDeclarator2> d){
    if(d==nullptr) return true;
    return d->declarator==nullptr && d->list==nullptr;
}
Var buildVar(std::shared_ptr<DirectDeclarator> p, Var returnVal){
    //std::cout<<"Direct declarator\n";
    if(isEmpty(p->direct_declarator)){
        if(p->declarator!=nullptr){
            return buildVar(p->declarator,returnVal);
        }
        else {
            returnVal.name=p->identifier;
            return returnVal;
        }
    }
    else{
        if(p->direct_declarator->list!=nullptr){
            // Hurray it is a func!
            auto params = buildParameters(p->direct_declarator->list);
            returnVal.params=std::make_shared<ParametersInfo>(params);
            returnVal.type=std::make_shared<const CFunctionType>(returnVal.type,returnVal.params->types);
        }
        if(p->declarator!=nullptr){
            return buildVar(p->declarator,returnVal);
        }
        else {
            returnVal.name=p->identifier;
            return returnVal;
        }
    }
}
// DONE!
Var buildVar(std::shared_ptr<Pointer> p, Var returnVal){
    //std::cout<<"pointer\n";
    if(p==nullptr)
    return returnVal;
    returnVal.type=returnVal.type->addStar();
    return buildVar(p->ptr,returnVal);
}
// DONE!
Var buildVar(std::shared_ptr<Declarator> d, Var returnVal){
    auto a = buildVar(d->ptr,returnVal);
    return buildVar(d->dec,a); // a is a return type
}
void CodeGen::visit(const c4::model::declaration::FunctionDefinition& s){
    Var a =  buildFromDS(s.ds);
    auto f = buildVar(s.declarator,a);
    if(f.type==nullptr){
        reportError(s.firstTerminal, "Not a valid type");
    }
    auto fu = std::dynamic_pointer_cast<const CFunctionType>(f.type);
    if(fu==nullptr){
        reportError(s.firstTerminal,"Not a function type.");
        return;
    }
    for(uint i=0;i<f.params->types.size();i++){
        if(f.params->types[i]->isVoid()){
            if(f.params->types.size()==1){
                f.params->types.clear();
                f.params->names.clear();
                fu=std::make_shared<const CFunctionType>(fu->retType,f.params->types);
            }
            else{
                reportError(s.firstTerminal,"Invalid parameter declaration");
                return;
            }
        }
    }
    // Function* func = Function::Create(
    //     fu->getLLVMFuncType(ctx), 
    //     GlobalValue::ExternalLinkage,
    //     f.name,
    //     &M
    // );
    const std::vector<std::shared_ptr<const CType>> val =f.params->types;
    currentFunc = fu;
    Function* func=nullptr;
    if(scope.isVarDeclared(f.name)){
        CTypedValue fun = scope[f.name];
        if(!fun.type->equivalent(&(*fu))){ // ???
            reportError(s.firstTerminal,"Redefinition of incompatible type!");
            return;
        }
        else{
            if(fun.type->isFuncNonDesignator()){
            Value* a = fun.value;
            std::cout<<"before\n";
            Function* val_func = static_cast<llvm::Function*>(a);
            std::cout<<"after\n";
            if(val_func!=nullptr){
                if(val_func->isDeclaration()){
                    func = val_func;
                }
                else{
                    reportError(s.firstTerminal,"Redefinition of a function!");
                }
            }
            else{
                // hopefully we never get there
            }
            }
        }

    } else{
        func = Function::Create(
        fu->getLLVMFuncType(ctx), 
        GlobalValue::ExternalLinkage,
        f.name,
        &M
        );
        scope.declareVar(
            f.name,
            CTypedValue(func, fu)
    );
    }
    scope.pushScope();

    BasicBlock* funcAddBlockEntry = BasicBlock::Create(
        ctx,
        f.name+"_entry",
        func,
        NULL //Insert at the end
    );
    allocaBuilder.SetInsertPoint(funcAddBlockEntry);
    builder.SetInsertPoint(funcAddBlockEntry);
    FirstPhase=true;
    s.statement->accept(*this);
    FirstPhase=false;
    for(uint i=0; i<fu->paramTypes.size(); i++) {
        if(f.params->names[i]==""){
            reportError(s.firstTerminal,"Parameter without name");
        }
        Argument* arg = func->arg_begin()+i;
        arg->setName(f.params->names[i]);
        AllocaInst *lvalue = Alloca(arg->getType(), f.params->names[i]);
        builder.CreateStore(arg, lvalue);
        CTypedValue typedLvalue(lvalue, f.params->types[i]);
        if(scope.varAlreadyDeclared(f.params->names[i])){
            reportError(s.firstTerminal, "Redeclaration of parameter");
            continue;
        }
        scope.declareVar(f.params->names[i], typedLvalue); //Every time we use this we look at the map
    }
    s.statement->accept(*this);
    if (builder.GetInsertBlock()->getTerminator() == nullptr) {
        Type *CurFuncReturnType = builder.getCurrentFunctionReturnType();
        if (CurFuncReturnType->isVoidTy()) {
            builder.CreateRetVoid();
        } else {
            builder.CreateRet(Constant::getNullValue(CurFuncReturnType));
    }
  }
    gotoLabels.clear();
    verifyFunction(*func);

}
void CodeGen::visit(const c4::model::declaration::Declaration& s){
    if(FirstPhase) return;
    Var f =  buildDeclarationFromDS(s.ds);
    if (s.declarator!=nullptr)
    f = buildVar(s.declarator,f);
    else{
        if(!(f.type->isStruct()))
        reportError(s.firstTerminal,"Declaration with declarator is not allowed.");
    }

    if(f.type==nullptr) std::cout<< "dupa1\n";
    if(f.type->isStruct()){
        auto fu = std::dynamic_pointer_cast<const CStructType>(f.type);
        if(fu==nullptr) {
            std::cout << "dupa\n"; 
            return;
        }
        if(fu->fieldNames.empty() && SecondPhase){
            scope.declareStruct(f.structname);
            return;
        }
        else if(SecondPhase && !fu->fieldNames.empty()){
            if(f.structname=="") return;
            if(scope.structAlreadyDefined(f.structname)){
                reportError(s.firstTerminal,"Redefinition of struct!");
                return;
            }
            scope.defineStruct(f.structname, fu);
            return;
        }
        else if(fu->fieldNames.empty() && !SecondPhase){
            if(f.structname!=""){
                if(scope.isStructDefined(f.structname)){
                    auto a = scope[f.structname];
                    if(scope.isGlobal()){
                        scope.declareVar(f.name, CTypedValue(GlobalAlloca(fu,f.name),fu));
                    }
                    else{
                        scope.declareVar(f.name, CTypedValue(Alloca(fu,f.name),fu));
                    }
                }
                else{
                    if(fu->indirections==0){
                    reportError(s.firstTerminal, "Missing definition for struct.");
                    return;
                    }
                    scope.declareStruct(f.name);
                    if(scope.isGlobal()){
                        scope.declareVar(f.name, CTypedValue(GlobalAlloca(fu,f.name),fu));
                    }
                    else{
                        scope.declareVar(f.name, CTypedValue(Alloca(fu,f.name),fu));
                    }
                }
            }
        }        
        else{
            if(f.structname!=""){
                if(scope.isStructDefined(f.structname)){
                    if(scope.isGlobal()){
                        scope.declareVar(f.name, CTypedValue(GlobalAlloca(fu,f.name),fu));
                    }
                    else{
                        scope.declareVar(f.name, CTypedValue(Alloca(fu,f.name),fu));
                    }
                }
                else{
                    // cannot happen
                    return;
                }
            }
            else{
                // Anonymous struct
                if(scope.isGlobal()){
                        scope.declareVar(f.name, CTypedValue(GlobalAlloca(fu,f.name),fu));
                    }
                    else{
                        scope.declareVar(f.name, CTypedValue(Alloca(fu,f.name),fu));
                    }
            }
        }
    }
    else if(f.type->isFuncNonDesignator()){
        if(SecondPhase) return;
        auto fu = std::dynamic_pointer_cast<const CFunctionType>(f.type);
        for(uint i=0;i<f.params->types.size();i++){
            if(f.params->types[i]->isVoid()){
                if(f.params->types.size()==1){
                    f.params->types.clear();
                    f.params->names.clear();
                    fu=std::make_shared<const CFunctionType>(fu->retType,f.params->types);
                }
                else{
                    reportError(s.firstTerminal,"Invalid parameter declaration");
                    return;
                }
            }
        }
        if(scope.isVarDeclared(f.name)){
            CTypedValue fun = scope[f.name];
            if(!fun.type->equivalent(&(*fu))){ // ???
                reportError(s.firstTerminal,"Redeclaration of incompatible type!");
            }
            else{
                // Function was already declared, dont have to do anything;
            }
        }else{
            Function* func = Function::Create(
                fu->getLLVMFuncType(ctx), 
                GlobalValue::ExternalLinkage,
                f.name,
                &M
                );
            const std::vector<std::shared_ptr<const CType>> val =f.params->types;
            currentFunc = fu;
            scope.declareVar(
                f.name,
                CTypedValue(func, fu)
                );
        } 
    }
    else{
        if(SecondPhase) return;
        if(scope.varAlreadyDeclared(f.name)){
                std::string msg="Variable with name: " + f.name + " was already declared in this scope";
                reportError(s.firstTerminal,msg);
            }
        else{
            if(!scope.isGlobal()){
                scope.declareVar(
                        f.name,
                        CTypedValue(Alloca(f.type->getLLVMType(ctx),f.name),f.type)
                    );
            }
            else{
                scope.declareVar(
                        f.name,
                        CTypedValue(GlobalAlloca(f.type->getLLVMType(ctx),f.name),f.type)
                    );
            }
        }
    }
}
void CodeGen::visit(const c4::model::declaration::ParameterDeclaration& s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::ParameterTypeList& s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::Pointer& s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::Root & s){
    // std::cout <<"Root\n";
    for(auto& a : s.definitions){
        a->accept(*this);
    }
}
void CodeGen::visit(const c4::model::declaration::StructDeclarationList & s){
    auto a =s; // WE DO NOTHING
    return; 
}
void CodeGen::visit(const c4::model::declaration::StructUnionSpecifier & s){
    auto a =s; // WE DO NOTHING
    return;
}
void CodeGen::visit(const c4::model::declaration::TypeName &s){
    auto a =s; // WE DO NOTHING
    return;
}


std::shared_ptr<const c4::model::ctype::CType> CodeGen::getCtype(std::shared_ptr<c4::model::declaration::TypeName> &s)
{
    Var a;
    a = buildDeclarationFromDS(s->ds);
    if(s->declarator!=nullptr){
        a = buildVar(s->declarator,a);
    }
    return a.type;
}