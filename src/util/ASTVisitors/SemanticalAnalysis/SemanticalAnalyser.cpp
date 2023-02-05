#include "SemanticalAnalyser.h"
#include "../../token/PrintVisitor.h"
using namespace c4::util::sema;
using namespace c4::model::declaration;
using namespace c4::model::statement;
using namespace c4::model::token;

void c4::util::sema::SemanticalAnalyser::reportError(std::shared_ptr<const model::token::Token> t, std::string msg)
{
    util::token::PrintVisitor v(os);
    v.printPosition(*t);
    os<<"error: " <<msg<<"\n";
    errorFound=true;
}
// DONE!
void c4::util::sema::SemanticalAnalyser::visit(const model::statement::CompoundStatement &s)
{
    declared_vars.push_back(std::set<std::string>());
    variables_type.push_back(std::map<std::string,type>());
    auto a= s;
    for(auto& a : s.block_of_statements){
        a->accept(*this);
    }
    declared_vars.pop_back();
    variables_type.pop_back();
}

void c4::util::sema::SemanticalAnalyser::visit(const model::statement::ExpressionStatement &s)
{
    auto a= s;
}

void c4::util::sema::SemanticalAnalyser::visit(const model::statement::IterationStatement &s)
{
    //INSPECT EXPR
    inLoop++;
    if(s.statement!=nullptr){
        s.statement->accept(*this);
    }
    inLoop--;
}

void c4::util::sema::SemanticalAnalyser::visit(const model::statement::JumpStatement &s)
{
    if(s.k==kind::_return){
        // INSPECT EXPR
    }
    else if(s.k==kind::_goto){
       if(labels.insert(std::make_pair(s.gotoIdentifier, false)).second){
        gotoLabels[s.gotoIdentifier]=s.firstTerminal;
       }
    }
    else{
        if(inLoop)
        return;
        else{
            this->reportError(s.firstTerminal,"continue and break are only allowed inside a loop");
        }
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::statement::LabeledStatement &s)
{
    auto a = s;
    if(labels.find(s.identifier)!=labels.end()){
        std::string msg = "this label:  " + s.identifier +" was already declared";
        if(labels[s.identifier]==true) reportError(s.firstTerminal,msg);
    }
    labels[s.identifier]=true;
    s.statement->accept(*this);
}

void c4::util::sema::SemanticalAnalyser::visit(const model::statement::SelectionStatement &s)
{
    // EXPR
    auto a= s;
    if(s.thenStatement!=nullptr) s.thenStatement->accept(*this);
    if(s.elseStatement!=nullptr) s.elseStatement->accept(*this);
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::Declaration &s)
{
    auto a = s;
    being_built.ptr_num=0;
    s.ds->accept(*this);
    if(s.declarator!=nullptr) s.declarator->accept(*this);
    else if(being_built.simple!=simple_type::_STRUCT){
        reportError(s.firstTerminal,"Declarations without declarators are not valid.");
    }
    if(isVar){
        variables_type.back()[name]=being_built;
        // os<< name << being_built.ptr_num;
        // switch(being_built.simple){
        //     case simple_type::_CHAR: os <<"char";
        //     break;
        //     case simple_type::_INT: os <<"int";
        //     break;
        //     case simple_type::_VOID: os <<"void";
        //     break;
        //     case simple_type::_STRUCT: os <<"struct";
        //     break;
        // }
        // os<<"\n";
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::DeclarationSpecifier &s)
{
    auto a= s;
    if(s.structorunion!=nullptr){
        being_built.simple=simple_type::_STRUCT;
        s.structorunion->accept(*this);
    }
    else{
        switch(s.keyword){
            case Keyword::Int: being_built.simple=simple_type::_INT;
            break;
            case Keyword::Char: being_built.simple=simple_type::_CHAR;
            break;
            case Keyword::Void: being_built.simple=simple_type::_VOID;
            break;
            default:
            break;
        }
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::Declarator &s)
{
    auto a= s;
    if(s.ptr!=nullptr) s.ptr->accept(*this);
    if(s.dec!=nullptr) s.dec->accept(*this);
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::DirectDeclarator &s)
{
    auto a= s;
    
    if(s.identifier!=""){
        auto se = declared_vars.back();
        if(se.find(s.identifier)!=se.end()){
            std::string msg="Variable with this name: \"" + s.identifier + "\" was already declared in this scope";            
            reportError(s.firstTerminal,msg);
            return;
        }
        else{
            se.insert(s.identifier);
            declared_vars.pop_back();
            declared_vars.push_back(se);
        }
    }
    ahead = true;
    if(s.direct_declarator!=nullptr){
        s.direct_declarator->accept(*this);
    }
    ahead=false;
    if(s.identifier!=""){
        name=s.identifier;
    }
    if(s.declarator!=nullptr){
        s.declarator->accept(*this);
    }

}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::DirectDeclarator2 &s)
{
    auto a= s;
    if(ahead){
        isVar= !(s.declarator!=nullptr || s.list!=nullptr);
        return;
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::FunctionDefinition &s)
{
    auto a= s;
    labels.clear();
    gotoLabels.clear();
    // INSPECT BODY
    //s.ds->accept(*this);
    being_built.ptr_num=0;
    //s.declarator->accept(*this);
    // PUT INTO THE DICTIONARY
    s.statement->accept(*this);
    //CHECK IF ALL LABELS WERE PRESENT
    for(auto it=labels.begin();it!=labels.end();it++){
        if(!it->second){
            std::string msg="label:\"" + it->first+ "\" not present";
            reportError(gotoLabels[it->first],msg);
        }
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::ParameterDeclaration &s)
{
    auto a= s;
}
void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::ParameterTypeList &s)
{
    auto a = s;
    inParameter=true;
    for(auto& a : s.params){
        a->accept(*this);
    }
    inParameter=false;
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::Pointer &s)
{
    auto a= s;
    being_built.ptr_num+=1;
    if(s.ptr!=nullptr){
        s.ptr->accept(*this);
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::Root &s)
{
    auto a= s;
    for(auto& a : s.definitions){
        a->accept(*this);
    }
}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::StructDeclarationList &s)
{
    auto a= s;

}

void c4::util::sema::SemanticalAnalyser::visit(const model::declaration::StructUnionSpecifier &s)
{
    auto a= s;

}
