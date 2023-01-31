#include "LLParser.h"
#include "../../util/expression/PrintVisitor.h"
#include "../../util/parser/lr/StateUtilities.h"
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
    //if(!inlookahead) std::cout<<"Consumed:";  
    (void) inlookahead;  
    auto a = m_input->read(&token);
    if(!a) {
        //s//td::cout<<"EOF\n" << token->position.column;
        token->accept(visitor);
        //if(visitor.getKind()==TokenKind::eof){
        //    std::cout<<"dupa";
        //};
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
    if(this->parse()==nullptr){
        util::token::PrintVisitor v(std::cerr);
        //token->accept(v);
        v.printPosition(*token);
        std::cerr<<"error: wrong token\n";
        return 1;
    }
    else{
        return 0;
    }
}

std::shared_ptr<Root> LLParser::parse()
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
    if(ds==nullptr) return nullptr;
    dec=parseDeclarator();
    if(dec==nullptr) return nullptr;    
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
        cs=parseCompoundStatement();
        if(cs==nullptr) return nullptr;
        decs.push_back(std::make_shared<IDeclaration>(std::make_shared<FunctionDefinition>(ds,dec,cs)));
    }
    else{
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return  nullptr;
        decs.push_back(std::make_shared<IDeclaration>(std::make_shared<Declaration>(ds,dec)));
    }
        m_input->pushMark();
        a = m_input->read(&token);
        m_input->resetAndPopMark();
    }while(a);
    return std::make_shared<Root>(decs);
}

std::shared_ptr<Declaration> LLParser::parseDeclaration()
{

    //First lets parse the type specifier
    //std::cout << "parseDeclaration()\n";
        auto a =parseDeclarationSpecifier();
        std::shared_ptr<Declarator> p =nullptr;
        if(a==nullptr) return nullptr; //TODO
        if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))
        {
            p=parseDeclarator();
            if(p==nullptr) return nullptr;
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return nullptr;
        return std::make_shared<Declaration>(a,p); //TODO
}

// bool LLParser::parseStaticAssertDeclaration()
// {
   
//     if(this->consume(TokenKind::keyword,SpecifiedToken(Keyword::__Static_assert))) return 1;

//     if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
//     // @TODO INVOKE LR PARSER HERE TO PARSE CONSTANT EXPRESSION

//     if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return 1;

//     if(this->consume(TokenKind::string_literal)) return 1;

//     if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;

//     if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return 1;


//     return 0;
// }

std::shared_ptr<StructUnionSpecifier> LLParser::parseStructorUnionSpecifier(){
    m_input->read(&token);
    token->accept(visitor);
    //std::cout<<"Struct!\n";
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;
    //std::cout<<"Struct!\n";

    if((visitor.getSepcificValue().k!=Keyword::Struct) 
    && (visitor.getSepcificValue().k!=Keyword::Union)) return nullptr;
    //std::cout<<"Struct!\n";
    if(checkLookAhead(TokenKind::identifier)){
        consume(TokenKind::identifier);
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace));
            if(parseStructDeclarationList()) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return nullptr;
            return 0;
        }
        else{
            return 0;
        }
    }
    else{
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))) return nullptr;
            if(parseStructDeclarationList()) return nullptr;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return nullptr;
            return 0;
    }

}
// TODO PARSE STRUCT DECLARATION
std::shared_ptr<StructDeclarationList> LLParser::parseStructDeclarationList(){

    do{
    if(parseDeclarationSpecifier()) return nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    }
    else{
        bool comma=true;
        while(comma){
        if(parseDeclarator()) return nullptr;
        if(checkLookAhead(TokenKind::punctuator, SpecifiedToken(Punctuator::Comma))){
            consume(TokenKind::punctuator, SpecifiedToken(Punctuator::Comma));
        }
        else{
            comma=false;
        }
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))return nullptr;
    }
    }while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace)));
    return 0;

}
// DONE!
std::shared_ptr<Pointer> c4::service::parser::LLParser::parsePointer()
{
    //Consume asterisk
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 1;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 1;
    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) return nullptr;
    //Check if we should parse another pointer;
    // m_input->pushMark();
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 0;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 0;
    // m_input->resetToMark();
    // m_input->popMark();
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) return std::make_shared<Pointer>(nullptr);
    auto ptr = parsePointer();
    if(ptr==nullptr) return nullptr;
    return std::make_shared<Pointer>(ptr);
}
// DONE!
std::shared_ptr<Declarator> c4::service::parser::LLParser::parseDeclarator()
{
    std::shared_ptr<Pointer> ptr=nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        ptr=parsePointer();
        if(ptr==nullptr) return nullptr;
    }
    //std::cout<< "Direct declarator\n";
    std::shared_ptr<DirectDeclarator> decl=parseDirectDeclarator();
    if(decl==nullptr) return nullptr;
    return std::make_shared<Declarator>(ptr,decl);
}

std::shared_ptr<Declarator> c4::service::parser::LLParser::parseAbstractDeclarator()
{
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) {
        if(parsePointer()) return nullptr;
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        if(parseDirectAbstractDeclarator()) return nullptr;
    }
    else{
        return 0;
    }
    return 0;
}
// TODO
std::shared_ptr<DirectDeclarator> c4::service::parser::LLParser::parseDirectAbstractDeclarator()
{
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) 
    return nullptr;
    std::shared_ptr<ParameterTypeList> p =nullptr;
    std::shared_ptr<Declarator> d =nullptr;

    if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Struct))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Union)))
    {
        p=parseParameterTypeList();
        if(p==nullptr) return nullptr;
    }
    else if(checkLookAhead(TokenKind::punctuator, SpecifiedToken(Punctuator::RightParenthesis))){}
    else{
        d=parseAbstractDeclarator();
        if(d==nullptr) return nullptr;
    }
    if (consume(TokenKind::punctuator, SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    auto b = parseDirectAbstractDeclarator2();
    if(b==nullptr) return nullptr;
    return std::make_shared<DirectDeclarator>("",d,b);

}

std::shared_ptr<DirectDeclarator2> c4::service::parser::LLParser::parseDirectAbstractDeclarator2()
{
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) 
    return std::make_shared<DirectDeclarator2>(nullptr,nullptr);
    consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis));
    std::shared_ptr<ParameterTypeList>p=nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis));
    }
    else{
        p = parseParameterTypeList();
        if(p==nullptr) return nullptr;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    }
    auto b = parseDirectAbstractDeclarator2();
    if(b==nullptr) return nullptr;
    return std::make_shared<DirectDeclarator2>(p,b);

}

std::shared_ptr<DirectDeclarator> c4::service::parser::LLParser::parseDirectDeclarator()
{
    // std::cout<< "parseDirectDeclarator()\n";
    // std::cout<< "parseDirectDeclarator()\n";
    //visit();
    std::string val="";
    std::shared_ptr<Declarator> declarator=nullptr;
    std::shared_ptr<DirectDeclarator2> declarator2=nullptr;

    if(checkLookAhead(TokenKind::identifier)){
        if(consume(TokenKind::identifier)) return nullptr;
        val=visitor.getVal();
    }
    else if (checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return nullptr;
        declarator=parseDeclarator();
        if(declarator==nullptr) return nullptr;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    }
    declarator2=parseDirectDeclarator2(); 
    return std::make_shared<DirectDeclarator>(val,declarator,declarator2);
}
std::shared_ptr<DirectDeclarator2> c4::service::parser::LLParser::parseDirectDeclarator2()
{
    std::shared_ptr<ParameterTypeList> params=nullptr;
    std::shared_ptr<DirectDeclarator2> declarator=nullptr;

    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 0;
    else{
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return nullptr;
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))){
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
            return std::make_shared<DirectDeclarator2>(params,nullptr);
        } 
        params=parseParameterTypeList();
        if(params==nullptr) return nullptr;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return nullptr;
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        declarator=parseDirectDeclarator2();
        if(declarator==nullptr) return nullptr;
        return std::make_shared<DirectDeclarator2>(params,declarator);
    }
    return std::make_shared<DirectDeclarator2>(params,declarator);
        // if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis)))
        // {
        //     consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis));
        // }
        // else if(visitor.getKind()==TokenKind::identifier){
        //     if(parseIdentifierList()) return 1;
        //     if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
        // }
        // else{
        //     if(parseParameterTypeList()) return 1;
        //     if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
        // }
   // }
   // else if(visitor.getKind()==TokenKind::punctuator && visitor.getSepcificValue().p==Punctuator::LeftBracket){
   //     consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBracket));
   //     if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket))){
   //         consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket));
   //     }
   //     else if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Static))){
   //         consume(TokenKind::keyword,SpecifiedToken(Keyword::Static));
   //         visit();
   //         if(visitor.getKind()==TokenKind::keyword){
   //             if(visitor.getSepcificValue().k==Keyword::Void || visitor.getSepcificValue().k==Keyword::Int || visitor.getSepcificValue().k==Keyword::Char ||visitor.getSepcificValue().k==Keyword::Struct)
   //             {
   //                 if(parseDeclarationSpecifier()) return 1;
   //             }
   //             // TODO PARSE ASSIGNMENT EXPR
   //         }
   //     }
   //     else if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))){
   //         consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk));
   //     }
   //     else if(visitor.getKind()==TokenKind::punctuator){
   //         // TODO PARSE ASSIGNMENT EXPR
   //     }
   //     else{
   //         if(parseDeclarationSpecifier()) return 1;
   //         if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Static))){
   //             consume(TokenKind::keyword,SpecifiedToken(Keyword::Static));
   //             // TODO PARSE ASSIGNMENT EXPR
   //         }
   //         else if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))){
   //             consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk));
   //         }
   //         else{
   //             // TODO PARSE ASSIGNMENT EXPR
   //         }
   //     }
   //     if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket))) return 1;
   // }
   // return 0;
}
// //DONE
std::shared_ptr<ParameterTypeList> c4::service::parser::LLParser::parseParameterTypeList()
{
    std::vector<std::shared_ptr<ParameterDeclaration>> params;
    auto a = parseParameterDeclaration();
    if(a==nullptr) return  nullptr;
    params.push_back(a);
    while(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma)))
    {
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma));
        a = parseParameterDeclaration();
        if(a==nullptr) return  nullptr;
        params.push_back(a);
    }
    return std::make_shared<ParameterTypeList>(params);
}
std::shared_ptr<ParameterDeclaration> c4::service::parser::LLParser::parseParameterDeclaration()
{
    auto spec = parseDeclarationSpecifier();
    if(spec==nullptr) return nullptr;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))||checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return 0;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        if(parsePointer()) return nullptr;
    }
    if(checkLookAhead(TokenKind::identifier)){
        if(parseDeclarator()) return nullptr;
    }
    else{
        if(parseAbstractDeclarator()) return nullptr;
    }
    
    return std::make_shared<ParameterDeclaration>();
}
std::shared_ptr<DeclarationSpecifier> c4::service::parser::LLParser::parseDeclarationSpecifier()
{
    visit();
    std::shared_ptr<StructUnionSpecifier> b=nullptr;
    Keyword key;
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;
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
        b = parseStructorUnionSpecifier();
        if(b==nullptr) return nullptr;
    }
    return std::make_shared<DeclarationSpecifier>(key,b);
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
    while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))){
    if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Struct))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Union))){

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
    return std::make_shared<CompoundStatement>(stmts);

}
// DONE
std::shared_ptr<SelectionStatement> c4::service::parser::LLParser::parseSelectionStatement()
{
    // std::cout << "parseSelectionStatement()\n";
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::If))) return nullptr;
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
        return std::make_shared<SelectionStatement>(b,ifstmt);}
    consume(TokenKind::keyword,SpecifiedToken(Keyword::Else));
    auto elsestmt = parseStatement();
    if(elsestmt==nullptr) return nullptr;
    return std::make_shared<SelectionStatement>(b,ifstmt,elsestmt);
}
// DONE
std::shared_ptr<IterationStatement> c4::service::parser::LLParser::parseIterationStatement()
{
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::While))) return nullptr;
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
    return std::make_shared<IterationStatement>(b,c);

}
// DONE
std::shared_ptr<JumpStatement> c4::service::parser::LLParser::parseJumpStatement()
{
    //std::cout<<"parseJumpStatement()\n";
    visit();
    std::string val="";
    std::shared_ptr<const IExpression> expr=nullptr;
    if(visitor.getKind()!=TokenKind::keyword) return nullptr;
    switch(visitor.getSepcificValue().k){
        case Keyword::Goto:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Goto));
            if(consume(TokenKind::identifier)) return nullptr;
            val=visitor.getVal();
            break;
        case Keyword::Continue:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Continue));
            break;
        case Keyword::Break:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Break));
            break;
        case Keyword::Return:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Return));
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
    return std::make_shared<JumpStatement>(expr,val);
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
       (std::make_shared<ExpressionStatement>(nullptr));
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
    return  std::static_pointer_cast<IStatement>(std::make_shared<ExpressionStatement>(b));
}

std::shared_ptr<LabeledStatement> c4::service::parser::LLParser::parseLabeledStatement()
{
    //std::cout<<"parseLabeledStatement\n";
    if(consume(TokenKind::identifier)) return nullptr;
    auto a =visitor.getVal();
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Colon))) return nullptr;
    auto b = parseStatement();
    if(b==nullptr) return nullptr;
    return std::make_shared<LabeledStatement>(a,b);
}

bool c4::service::parser::LLParser::visit()
{
    checkLookAhead(TokenKind::identifier);
    return false;
}
