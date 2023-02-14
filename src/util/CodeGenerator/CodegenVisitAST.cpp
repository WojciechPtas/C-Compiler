#include "../../service/CodeGenerator/codegen.h"

using namespace c4::model::statement;
using namespace c4::model::token;
using namespace c4::model::declaration;
using namespace c4::model::ctype;
using namespace llvm;
// DONE
void CodeGen::visit(const c4::model::statement::CompoundStatement& s){
    scope.pushScope();
    for(auto& a : s.block_of_statements){
        a->accept(*this);
    }
    scope.popScope();
    return;
}
// CODEGEN EXPR
void CodeGen::visit(const c4::model::statement::ExpressionStatement& s){
    if(FirstPhase) return;
}
// CODEGEN EXPR
void CodeGen::visit(const c4::model::statement::IterationStatement& s){
    if(FirstPhase){
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
        builder.SetInsertPoint(whileHeader);
       
        CTypedValue cond = s.expr->getRValue(*this);
        evaluateCondition(cond,false);
        if(!cond.isValid()) {
            //err
        }
        builder.SetInsertPoint(whileBody);
        // CODEGEN BODY
        if(s.statement!=nullptr) s.statement->accept(*this);
        builder.SetInsertPoint(afterWhile);
        headerBlocks.pop();
        afterBlocks.pop();
        return;
    }
}
void CodeGen::visit(const c4::model::statement::JumpStatement& s){
    if(FirstPhase) return;
    if(s.k==kind::_goto){
    if(gotoLabels.find(s.gotoIdentifier)!=gotoLabels.end()){
        builder.CreateBr(gotoLabels[s.gotoIdentifier]);
    }
    else{
        //report error
    }
    }
    else if(s.k==kind::_return){

    }
    else if(s.k==kind::_break){
        if(afterBlocks.size()!=0){
            builder.CreateBr(afterBlocks.top());
            return;
        }
    }
    else{ // _continue
        if(headerBlocks.size()!=0){
            builder.CreateBr(headerBlocks.top());
            return;
        }
        else{
            // report error
        }
    }
}
void CodeGen::visit(const c4::model::statement::LabeledStatement& s){
    if(FirstPhase){
        BasicBlock* jumpblock = BasicBlock::Create(
            ctx,
            s.identifier,
            builder.GetInsertBlock()->getParent(),
            NULL //Insert at the end
        );
        if(gotoLabels.find(s.identifier)!=gotoLabels.end()){
            //report error
        }
        else{
            gotoLabels[s.identifier]=jumpblock;
        }
    }
    else{
        builder.SetInsertPoint(gotoLabels[s.identifier]);
    }
    s.statement->accept(*this);
}
void CodeGen::visit(const c4::model::statement::SelectionStatement& s){
    if(FirstPhase){
        if(s.thenStatement!=nullptr) s.thenStatement->accept(*this);
        if(s.elseStatement!=nullptr) s.elseStatement->accept(*this);
    }
    else{

    }
}
void CodeGen::visit(const c4::model::declaration::Declaration& s){
    if(FirstPhase) return;

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
    std::shared_ptr<const CType> type;
    std::shared_ptr<ParametersInfo> params;
};

Var buildFromDS(std::shared_ptr<DeclarationSpecifier> ds){
    Var a;
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
Var buildParam(std::shared_ptr<ParameterDeclaration> param);
ParametersInfo buildParameters(std::shared_ptr<ParameterTypeList> params);
Var buildVar(std::shared_ptr<DirectDeclarator> p, Var returnVal);
Var buildVar(std::shared_ptr<Pointer> p, Var returnVal);
Var buildVar(std::shared_ptr<Declarator> d, Var returnVal);

Var buildParam(std::shared_ptr<ParameterDeclaration> param){
    auto base = buildFromDS(param->type);
    return buildVar(param->dec,base);
}
// DONE!
ParametersInfo buildParameters(std::shared_ptr<ParameterTypeList> params){
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
    if(isEmpty(p->direct_declarator)){
        if(p->declarator!=nullptr){
            return buildVar(p->declarator,returnVal);
        }
        else if(p->identifier!=""){
            returnVal.name=p->identifier;
            return returnVal;
        }
    }
    else{
        if(p->direct_declarator->list!=nullptr){
            // Hurray it is a func!
            auto params = buildParameters(p->direct_declarator->list);
            returnVal.params=std::make_shared<ParametersInfo>(params);
        }
        if(p->declarator!=nullptr){
            return buildVar(p->declarator,returnVal);
        }
        else if(p->identifier!=""){
            returnVal.name=p->identifier;
            return returnVal;
        }
    }
}
// DONE!
Var buildVar(std::shared_ptr<Pointer> p, Var returnVal){
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
    std::cout<< "Function Definition!\n";
    FirstPhase=true;
    s.statement->accept(*this);
    FirstPhase=false;
    std::cout << "after first run\n";
    Var a =  buildFromDS(s.ds);
    auto f = buildVar(s.declarator,a);
    auto fu = std::dynamic_pointer_cast<const CFunctionType>(f.type);
    std::cout << "build dec\n";
    Function* func = Function::Create(
        fu->getLLVMFuncType(ctx), 
        GlobalValue::ExternalLinkage,
        f.name,
        &M
    );
    std::cout<<"Lets go statements!\n";
    const std::vector<std::shared_ptr<const CType>> val =f.params->types;
    scope.declareVar(
        f.name,
        CTypedValue(func, std::make_shared<CFunctionType>(fu, val))
    );
    std::cout<<"Lets go statements!\n";
    scope.pushScope();

    BasicBlock* funcAddBlockEntry = BasicBlock::Create(
        ctx,
        f.name+"_entry",
        func,
        NULL //Insert at the end
    );
    allocaBuilder.SetInsertPoint(funcAddBlockEntry);
    builder.SetInsertPoint(funcAddBlockEntry);
    std::cout<<"Lets go statements!\n";
    for(uint i=0; i<fu->paramTypes.size(); i++) {
        Argument* arg = func->arg_begin()+i;
        arg->setName(f.params->names[i]);
        AllocaInst *lvalue = Alloca(arg->getType());
        builder.CreateStore(arg, lvalue);
        CTypedValue typedLvalue(lvalue, f.params->types[i]);
        scope.declareVar(f.params->names[i], typedLvalue); //Every time we use this we look at the map
    }
    std::cout<<"Lets go statements!\n";
    s.statement->accept(*this);
    gotoLabels.clear();

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
    std::cout <<"Root\n";
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