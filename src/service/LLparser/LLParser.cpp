#include "LLParser.h"
#include "../../util/expression/PrintVisitor.h"
#include "../../util/parser/lr/StateUtilities.h"
#include "../../util/ASTVisitors/PrettyPrintingVisitor/PrettyPrintingVisitor.h"
#include "../../util/ASTVisitors/SemanticalAnalysis/SemanticalAnalyser.h"

#include <iostream>
using namespace c4::service::io;
using namespace c4::util::token;
using namespace c4::service::parser;
using namespace c4::model::token;
using namespace c4::util::parser::lr;
using namespace c4::util::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::statement;
using namespace c4::model::declaration;
using namespace c4::model::expression;

// DONE!
bool LLParser::checkLookAhead(TokenKind k, SpecifiedToken s)
{
    //std::cout<<"In lookahead:";
    m_input->pushMark();
    bool seen = !consume(k,s,true);
    m_input->resetToMark();
    m_input->popMark();
    return seen;
}

//DONE!
bool LLParser::consume(TokenKind k, SpecifiedToken s, bool inlookahead)
{
    (void) inlookahead;  
    auto a = m_input->read(&token);
    if(!a) {
        if(token!=nullptr)
        token->accept(visitor);
        return 1;
    }
    token->accept(visitor);
        if(token->isError()){
        errorcode=1;
        return 1;
    }
    if(visitor.getKind()!=k) return 1;
    switch(k){
        case TokenKind::keyword:
        return !(visitor.getSepcificValue().k==s.k);
        case TokenKind::punctuator:
        if(s.p==Punctuator::LeftBrace){
            if(visitor.getSepcificValue().p==Punctuator::LessThanPercent) return 0;
        }
        else if(s.p==Punctuator::RightBrace){
            if(visitor.getSepcificValue().p==Punctuator::PercentGreaterThan) return 0;
        }
        return !(visitor.getSepcificValue().p==s.p);
        default:
        return 0;
    }
}

int c4::service::parser::LLParser::run()
{
    auto a =this->parse();
    if(a==nullptr){
        util::token::PrintVisitor v(std::cerr);
        //token->accept(v);
        v.printPosition(*token);
        std::cerr<<"error: wrong token\n";
        return 1;
    }
    else{
        util::sema::SemanticalAnalyser sem(std::cerr);
        a->accept(sem);
        return sem.isErrorFound();
        //return 0;
    }
}

int c4::service::parser::LLParser::print()
{
    auto a=this->parse();
    if(a==nullptr){
        util::token::PrintVisitor v(std::cerr);
        //token->accept(v);
        v.printPosition(*token);
        std::cerr<<"error: wrong token\n";
        return 1;
    }
    else{
        util::sema::SemanticalAnalyser sem(std::cerr);
        a->accept(sem);
        if(sem.isErrorFound()) return 1;
        util::pretty::PrettyPrintinVisitor p(std::cout);
        a->accept(p);
        return 0;
    }
}

std::shared_ptr<IDeclaration> LLParser::parse()
{
    m_input->pushMark();
    auto a = m_input->read(&token);
    m_input->resetAndPopMark();
    std::vector<std::shared_ptr<IDeclaration>> decs;
    std::shared_ptr<DeclarationSpecifier> ds=nullptr;
    std::shared_ptr<Declarator> dec=nullptr;
    std::shared_ptr<CompoundStatement> cs=nullptr;

    do{
        ds=parseDeclarationSpecifier();
        if(ds==nullptr) 
            return nullptr;
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
             decs.push_back(std::static_pointer_cast<IDeclaration>(std::make_shared<Declaration>(ds,nullptr)));
        }else{
        dec=parseDeclarator();
        if(dec==nullptr) 
            return nullptr;    
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
            cs=parseCompoundStatement();
            if(cs==nullptr) return nullptr;
            decs.push_back(std::static_pointer_cast<IDeclaration>(std::make_shared<FunctionDefinition>(ds,dec,cs)));
        }
        else{
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return  nullptr;
            decs.push_back(std::static_pointer_cast<IDeclaration>(std::make_shared<Declaration>(ds,dec)));
        }}
            m_input->pushMark();
            a = m_input->read(&token);
            m_input->resetAndPopMark();
    }while(a);
    return std::static_pointer_cast<IDeclaration>(std::make_shared<Root>(decs));
}

std::shared_ptr<Declaration> LLParser::parseDeclaration()
{

        auto a =parseDeclarationSpecifier();
        std::shared_ptr<Declarator> p =nullptr;
        if(a==nullptr) return nullptr; 
        if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))
        {
            p=parseDeclarator();
            if(p==nullptr) return nullptr;
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return nullptr;
        return (std::make_shared<Declaration>(a,p)); 
}

std::shared_ptr<StructUnionSpecifier> LLParser::parseStructorUnionSpecifier(){
    m_input->read(&token);
    token->accept(visitor);
    auto first_token=token;
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;


    if((visitor.getSepcificValue().k!=Keyword::Struct) 
    && (visitor.getSepcificValue().k!=Keyword::Union)) return nullptr;

    if(checkLookAhead(TokenKind::identifier)){
        consume(TokenKind::identifier);
        std::string name= visitor.getVal();
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace));
            auto a =parseStructDeclarationList();
            if(a==nullptr) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return nullptr;
            return 
            (std::make_shared<StructUnionSpecifier>(name,a,first_token));
        }
        else{
            return 
            (std::make_shared<StructUnionSpecifier>(name,nullptr,first_token));
        }
    }
    else{
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))) return nullptr;
            auto a =parseStructDeclarationList();
            if(a==nullptr) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return nullptr;
            return 
            (std::make_shared<StructUnionSpecifier>("",a,first_token));
    }

}
// DONE
std::shared_ptr<StructDeclarationList> LLParser::parseStructDeclarationList(){

    std::shared_ptr<DeclarationSpecifier> ds=nullptr;
    std::shared_ptr<Declarator> declarator=nullptr;
    std::vector<std::shared_ptr<Declaration>> vec;
    do{
        ds=parseDeclarationSpecifier();
        if(ds==nullptr) return nullptr;
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
            vec.push_back(
            (std::make_shared<Declaration>(ds,nullptr)));
        }
        else{
            declarator=parseDeclarator();
            if(declarator==nullptr) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))return nullptr;
            vec.push_back(
            (std::make_shared<Declaration>(ds,declarator)));
        }
    }while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace)));
    return 
            (std::make_shared<StructDeclarationList>(vec));

}
// DONE!
std::shared_ptr<Pointer> c4::service::parser::LLParser::parsePointer()
{
    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) return nullptr;
    auto first_token=token;
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    return (std::make_shared<Pointer>(nullptr,first_token));
    auto ptr = parsePointer();
    if(ptr==nullptr) return nullptr;
    return (std::make_shared<Pointer>(ptr,first_token));
}
// DONE!
std::shared_ptr<Declarator> c4::service::parser::LLParser::parseDeclarator(bool abstract)
{
    std::shared_ptr<Pointer> ptr=nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        ptr=parsePointer();
        if(ptr==nullptr) return nullptr;
    }
    //std::cout<< "Direct declarator\n";
    std::shared_ptr<DirectDeclarator> decl=parseDirectDeclarator(abstract);
    if(decl==nullptr) return nullptr;
    return (std::make_shared<Declarator>(ptr,decl));
}

std::shared_ptr<DirectDeclarator> c4::service::parser::LLParser::parseDirectDeclarator(bool abstract)
{
    std::string val="";
    std::shared_ptr<Declarator> declarator=nullptr;
    std::shared_ptr<DirectDeclarator2> declarator2=nullptr;
    std::shared_ptr<Token> first_token=nullptr;
    m_input->pushMark();
    if(checkLookAhead(TokenKind::identifier)){
        if(consume(TokenKind::identifier)) return nullptr;
        val=visitor.getVal();
        first_token=token;
        m_input->popMark();
    }
    else if (checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return nullptr;
        first_token=token;
        if(!(((checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))
            ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))
            ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))
            ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Struct))
            ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Union)))
           )&&(abstract))){
            m_input->popMark();
            declarator=parseDeclarator(abstract);
            if(declarator==nullptr) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
        }
        else{
            m_input->resetAndPopMark();
            declarator2=parseDirectDeclarator2();
            if(declarator2==nullptr) return nullptr;
            return (
            std::make_shared<DirectDeclarator>(val,declarator,declarator2,first_token)); 
        }
        
    }
    else if(abstract && (checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))
                     ||  checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))    )
    ){
        m_input->popMark();
        return (
        std::make_shared<DirectDeclarator>(val,declarator,declarator2,first_token));

    }
    else {m_input->popMark();
    return nullptr;} 
    declarator2=parseDirectDeclarator2(); 
    if(declarator2==nullptr) return nullptr;
    return (
        std::make_shared<DirectDeclarator>(val,declarator,declarator2,first_token));
}
std::shared_ptr<DirectDeclarator2> c4::service::parser::LLParser::parseDirectDeclarator2()
{
    std::shared_ptr<ParameterTypeList> params=nullptr;
    std::shared_ptr<DirectDeclarator2> declarator=nullptr;
    std::shared_ptr<Token> first_token=nullptr;
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis)))
    return std::make_shared<DirectDeclarator2>(nullptr,nullptr,nullptr);
    else{
        //std::cout<<"after Lparent\n";
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis)))
        return nullptr;
        first_token=token;
        // if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))){
        //     if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis)))
        //     return nullptr;
        //     return std::make_shared<DirectDeclarator2>(params,nullptr);
        // }
        if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) 
        {   
            params=parseParameterTypeList();
            if(params==nullptr) return nullptr;
        }
        else{
            std::vector<std::shared_ptr<ParameterDeclaration>> a;
            params=(std::make_shared<ParameterTypeList>
            ((a)));
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        declarator=(parseDirectDeclarator2());
        if(declarator==nullptr) return nullptr;
        return std::make_shared<DirectDeclarator2>(params,declarator,first_token);
    }
    return (
        std::make_shared<DirectDeclarator2>(params,declarator,first_token));
    
}
// //DONE
std::shared_ptr<ParameterTypeList> c4::service::parser::LLParser::parseParameterTypeList()
{
    std::vector<std::shared_ptr<ParameterDeclaration>> params;
    auto a = (parseParameterDeclaration());
    if(a==nullptr) return  nullptr;
    params.push_back(a);
    while(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma)))
    {
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma));
        a = (parseParameterDeclaration());
        if(a==nullptr) return  nullptr;
        params.push_back(a);
    }
    return (std::make_shared<ParameterTypeList>(params));
}
std::shared_ptr<ParameterDeclaration> c4::service::parser::LLParser::parseParameterDeclaration()
{
    auto spec = parseDeclarationSpecifier();
    std::shared_ptr<Declarator> aa=nullptr;
    //std::cout<<"Specifier\n";
    if(spec==nullptr) return nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))||checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma)))
     return (std::make_shared<ParameterDeclaration>(spec,nullptr));;
    //if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    //{
    //    if(parsePointer()) return nullptr;
    //}
    //if(checkLookAhead(TokenKind::identifier)){
        //std::cout<<"before dec\n";
        aa=parseDeclarator(true);
        if(aa==nullptr) return nullptr;
    //}
    //else{
    //    if(parseAbstractDeclarator()) return nullptr;
    //}
    
    return (std::make_shared<ParameterDeclaration>(spec,aa));
}
std::shared_ptr<DeclarationSpecifier> c4::service::parser::LLParser::parseDeclarationSpecifier()
{
    visit();
    std::shared_ptr<StructUnionSpecifier> b=nullptr;
    Keyword key;
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;
    std::shared_ptr<Token> first_token=token;
    if (visitor.getSepcificValue().k == Keyword::Int)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Int));
        key=Keyword::Int;
    }
    else if(visitor.getSepcificValue().k == Keyword::Void)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Void));
        key=Keyword::Void;
    }
    else if(visitor.getSepcificValue().k == Keyword::Char)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Char));
        key=Keyword::Char;
    }
    else{
        //std::cout<<"Dupa\n";
        key=Keyword::Struct;
        b = (parseStructorUnionSpecifier());
        if(b==nullptr) return nullptr;
    }
    return (std::make_shared<DeclarationSpecifier>(key,b,first_token));
}
// // DONE
// bool c4::service::parser::LLParser::parseIdentifierList()
// {
//     //  std::cout<<"parseIdentifierList()\n";
//     if(consume(TokenKind::identifier)) return 1;
//     if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) {
//         consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma));
//         return parseIdentifierList();
//         }
//     return 0;
// }
// DONE
std::shared_ptr<CompoundStatement> c4::service::parser::LLParser::parseCompoundStatement() // {dasdasd}
{
    //std::cout<<"parseCompoundStatement()\n";
    std::vector<std::shared_ptr<IStatement>> stmts;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))) return nullptr;
    std::shared_ptr<Token> first_token=token;
    while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))){
    if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))
        ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))
        ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))
        ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Struct))
        ||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Union))){

            auto dec= parseDeclaration();
            if(dec==nullptr) return nullptr;
            stmts.push_back(dec);
        }
        else{
            auto stm=parseStatement();
            if(stm==nullptr) return nullptr;
            stmts.push_back(stm);
        }
    }
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return nullptr;
    //std::cout<<"ParsedCompound\n";
    return std::make_shared<CompoundStatement>(stmts,first_token);

}
// DONE
std::shared_ptr<SelectionStatement> c4::service::parser::LLParser::parseSelectionStatement()
{
    // if (expr) {}
    // std::cout << "parseSelectionStatement()\n";
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::If))) return nullptr;
    std::shared_ptr<Token> first_token=token;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return nullptr;
    ParenthesisDelimiterStream stream(m_input);
    auto a=std::make_shared<State>(INITIAL_STATE);

    auto lrparser = std::make_shared<ExpressionParser>(a);
    auto b = lrparser->parse(stream);
    if(b==nullptr || lrparser->lastTokenRead()->isError()){
                token=lrparser->lastTokenRead();
                return nullptr;
                }
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    auto ifstmt= parseStatement();
    if(ifstmt==nullptr) return nullptr;
    if(!checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Else))){
        return std::make_shared<SelectionStatement>(first_token,b,ifstmt);}
    consume(TokenKind::keyword,SpecifiedToken(Keyword::Else));
    auto elsestmt = parseStatement();
    if(elsestmt==nullptr) return nullptr;
    return std::make_shared<SelectionStatement>(first_token,b,ifstmt,elsestmt);
}
// DONE
std::shared_ptr<IterationStatement> c4::service::parser::LLParser::parseIterationStatement()
{
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::While))) return nullptr;
    std::shared_ptr<Token> first_token=token;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return nullptr;
    ParenthesisDelimiterStream stream(m_input);
    auto a=std::make_shared<State>(INITIAL_STATE);
    auto lrparser = std::make_shared<ExpressionParser>(a);
    auto b = lrparser->parse(stream);
    if(b==nullptr || lrparser->lastTokenRead()->isError()){
                token=lrparser->lastTokenRead();
                return nullptr;
                }
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    auto c = parseStatement();
    if(c==nullptr) return nullptr;
    return std::make_shared<IterationStatement>(b,c,first_token);

}
// DONE
std::shared_ptr<JumpStatement> c4::service::parser::LLParser::parseJumpStatement()
{
    //std::cout<<"parseJumpStatement()\n";
    visit();
    std::string val="";
    std::shared_ptr<const IExpression> expr=nullptr;
    kind k;
    std::shared_ptr<Token> first_token=token;
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;
    switch(visitor.getSepcificValue().k){
        case Keyword::Goto:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Goto));
            if(consume(TokenKind::identifier)) return nullptr;
            val=visitor.getVal();
            k=kind::_goto;
            break;
        case Keyword::Continue:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Continue));
            k=kind::_continue;
            break;
        case Keyword::Break:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Break));
            k=kind::_break;
            break;
        case Keyword::Return:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Return));
            k=kind::_return;
        if(this->checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) break;
        else{
            //std::cout <<"Return\n";
            DelimiterStream stream(m_input, TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
            auto a=std::make_shared<State>(INITIAL_STATE);
            auto lrparser = std::make_shared<ExpressionParser>(a);
            auto b = lrparser->parse(stream);//->accept(a);
            if(b==nullptr || lrparser->lastTokenRead()->isError()){
                token=lrparser->lastTokenRead();
                return nullptr;
            }
            expr=b;
            //std::cout<<"works\n";
        }
        break;
        default:
        return nullptr;
    }
    if(consume(TokenKind::punctuator, SpecifiedToken(Punctuator::Semicolon))) return nullptr;
    return std::make_shared<JumpStatement>(expr,val,k,first_token);
}
// DONE
std::shared_ptr<IStatement> c4::service::parser::LLParser::parseStatement()
{
    // std::cout<<"parseStatement()\n";
    visit();
    if(visitor.getKind()==(TokenKind::identifier)){
        m_input->pushMark();
        m_input->read(&token);
        visit();
        m_input->resetAndPopMark(); // LL2
        if(visitor.getKind()==TokenKind::punctuator){
            if(visitor.getSepcificValue().p==Punctuator::Colon){
                return std::static_pointer_cast<IStatement>(parseLabeledStatement());
            }
        }
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
        return std::static_pointer_cast<IStatement>(parseCompoundStatement());
    }
    else if(visitor.getKind()==(TokenKind::keyword)){
        switch(visitor.getSepcificValue().k){
            case Keyword::If:
            return  std::static_pointer_cast<IStatement>(parseSelectionStatement());
            case Keyword::While:
            return  std::static_pointer_cast<IStatement>(parseIterationStatement());
            case Keyword::Goto:
            case Keyword::Break:
            case Keyword::Continue:
            case Keyword::Return:
            return  std::static_pointer_cast<IStatement>(parseJumpStatement());
            default:
            break;
        }
    }
    else if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
       consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
       return std::static_pointer_cast<IStatement>
       (std::make_shared<ExpressionStatement>(nullptr,nullptr));
    }
    DelimiterStream stream(m_input,TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    auto a=std::make_shared<State>(INITIAL_STATE);
    auto lrparser = std::make_shared<ExpressionParser>(a);
    auto b = lrparser->parse(stream);
    //std::cout<<"dupa\n";
    if(b==nullptr || lrparser->lastTokenRead()->isError()){
        token=lrparser->lastTokenRead();
        //std::cout<<"parsed\n";
        return nullptr;
    }
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return nullptr;
    return  std::static_pointer_cast<IStatement>(std::make_shared<ExpressionStatement>(b,nullptr)); //TODO
}

std::shared_ptr<LabeledStatement> c4::service::parser::LLParser::parseLabeledStatement()
{
    //std::cout<<"parseLabeledStatement\n";
    if(consume(TokenKind::identifier)) return nullptr;
    std::shared_ptr<Token> first_token=token;
    auto a =visitor.getVal();
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Colon))) return nullptr;
    auto b = parseStatement();
    if(b==nullptr) return nullptr;
    return std::make_shared<LabeledStatement>(a,b,first_token);
}

bool c4::service::parser::LLParser::visit()
{
    checkLookAhead(TokenKind::identifier);
    return false;
}
