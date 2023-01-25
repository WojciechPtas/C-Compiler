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
    if(this->parse()){
        util::token::PrintVisitor v(std::cout);
        //token->accept(v);
        v.printPosition(*token);
        std::cout<<"error: wrong token\n";
        return 1;
    }
    else{
        return 0;
    }
}

bool LLParser::parse()
{
    m_input->pushMark();
    auto a = m_input->read(&token);
    m_input->resetAndPopMark();
    do{
    if(parseDeclarationSpecifier()) return 1;
    if(parseDeclarator()) return 1;    
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
        if(parseCompoundStatement()) return 1;
    }
    else{
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return 1;
    }
        m_input->pushMark();
        a = m_input->read(&token);
        m_input->resetAndPopMark();
    }while(a);
    return 0;
}

bool LLParser::parseDeclaration()
{

    //First lets parse the type specifier
    //std::cout << "parseDeclaration()\n";

    visit();
    if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::__Static_assert))){
        return parseStaticAssertDeclaration();
    }
    else{
        if(parseDeclarationSpecifier()) return 1;
        if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))
        {
            if(parseDeclarator()) return 1;
        }
        return consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    }
}

bool LLParser::parseStaticAssertDeclaration()
{
   
    if(this->consume(TokenKind::keyword,SpecifiedToken(Keyword::__Static_assert))) return 1;

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
    // @TODO INVOKE LR PARSER HERE TO PARSE CONSTANT EXPRESSION

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return 1;

    if(this->consume(TokenKind::string_literal)) return 1;

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return 1;


    return 0;
}

bool LLParser::parseStructorUnionSpecifier(){
    m_input->read(&token);
    token->accept(visitor);
    //std::cout<<"Struct!\n";
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    //std::cout<<"Struct!\n";

    if((visitor.getSepcificValue().k!=Keyword::Struct) && (visitor.getSepcificValue().k!=Keyword::Union)) return 1;
    //std::cout<<"Struct!\n";
    if(checkLookAhead(TokenKind::identifier)){
        consume(TokenKind::identifier);
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace));
            if(parseStructDeclarationList()) return 1;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return 1;
            return 0;
        }
        else{
            return 0;
        }
    }
    else{
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))) return 1;
            if(parseStructDeclarationList()) return 1;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return 1;
            return 0;
    }

    
//     m_input->read(&token);
//     token->accept(visitor);
//     if(visitor.getKind()==TokenKind::identifier){
//         m_input->pushMark();
//         m_input->read(&token);
//         token->accept(visitor);
//         if(visitor.getKind()==TokenKind::punctuator){ 
//         if(visitor.getSepcificValue().p!=Punctuator::LeftBrace) {
//              m_input->popMark();
//             return 0;
//         }
//         if(this->parseStructDeclarationList()) return 1;
//         m_input->read(&token);
//         token->accept(visitor);
//         if(visitor.getKind()!=TokenKind::punctuator) return 1;
//         if(visitor.getSepcificValue().p!=Punctuator::RightBrace) return 1;
//         m_input->popMark();
//         return 0;
//     }
//     }
//     else if(visitor.getKind()==TokenKind::punctuator){ 
//         if(visitor.getSepcificValue().p!=Punctuator::LeftBrace) return 1;
//         m_input->popMark();
//         if(this->parseStructDeclarationList()) return 1;
//         if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return 1;
//         m_input->popMark();
//         return 0;
//     }
//    return 1;
}
// TODO PARSE STRUCT DECLARATION
bool LLParser::parseStructDeclarationList(){
    //ParserVisitor visitor;

    // TODO PARSE STRUCT DECLARATION
    //std::shared_ptr<const Token> token;
    // m_input->pushMark();
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()==TokenKind::punctuator){
    //     if(visitor.getSepcificValue().p==Punctuator::RightBrace){
    //         m_input->resetToMark();
    //         m_input->popMark();
    //         return 0;
    //     }
    //     return 1;
    // }
    // else{
    //     m_input->resetToMark();
    //     m_input->popMark();
    //      return this->parseStructDeclarationList();
    // }
    // return 1;
    do{
    if(parseDeclarationSpecifier()) return 1;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    }
    else{
        bool comma=true;
        while(comma){
        if(parseDeclarator()) return 1;
        if(checkLookAhead(TokenKind::punctuator, SpecifiedToken(Punctuator::Comma))){
            consume(TokenKind::punctuator, SpecifiedToken(Punctuator::Comma));
        }
        else{
            comma=false;
        }
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)))return 1;
    }
    }while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace)));
    return 0;

}
// DONE!
bool c4::service::parser::LLParser::parsePointer()
{
    //Consume asterisk
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 1;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 1;
    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) return 1;
    //Check if we should parse another pointer;
    // m_input->pushMark();
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 0;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 0;
    // m_input->resetToMark();
    // m_input->popMark();
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) return 0;
    return this->parsePointer();
}
// DONE!
bool c4::service::parser::LLParser::parseDeclarator()
{
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        if(parsePointer()) return 1;
    }
    //std::cout<< "Direct declarator\n";
    return parseDirectDeclarator();
}

bool c4::service::parser::LLParser::parseAbstractDeclarator()
{
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) {
        if(parsePointer()) return 1;
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        if(parseDirectAbstractDeclarator()) return 1;
    }
    else{
        return 0;
    }
    return 0;
}

bool c4::service::parser::LLParser::parseDirectAbstractDeclarator()
{
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
    if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Struct))
    || checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Union)))
    {if(parseParameterTypeList()) return 1;}
    else if(checkLookAhead(TokenKind::punctuator, SpecifiedToken(Punctuator::RightParenthesis))){}
    else{
        if(parseAbstractDeclarator()) return 1;
    }
    if (consume(TokenKind::punctuator, SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    if(parseDirectAbstractDeclarator2()) return 1;
    return 0;

}

bool c4::service::parser::LLParser::parseDirectAbstractDeclarator2()
{
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 0;
    consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis));
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis));
    }
    else{
        if(parseParameterTypeList()) return 1;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    }
    return parseDirectAbstractDeclarator2();

}

bool c4::service::parser::LLParser::parseDirectDeclarator()
{
    // std::cout<< "parseDirectDeclarator()\n";
    // std::cout<< "parseDirectDeclarator()\n";
    //visit();
    if(checkLookAhead(TokenKind::identifier)){
        if(consume(TokenKind::identifier)) return 1;
    }
    else if (checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
        if(parseDeclarator()) return 1;;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    }
    return parseDirectDeclarator2(); 
}
bool c4::service::parser::LLParser::parseDirectDeclarator2()
{
   
    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 0;
    else{
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))){
            return consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis));
        }
        if(parseParameterTypeList()) return 1;
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))){
        return parseDirectDeclarator2();
    }
    return 0;
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
bool c4::service::parser::LLParser::parseParameterTypeList()
{
    if(parseParameterDeclaration()) return 1;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma)))  {
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma));
        if(parseParameterTypeList()) return 1;;
    }
    return 0;
}
bool c4::service::parser::LLParser::parseParameterDeclaration()
{
    if(parseDeclarationSpecifier()) return 1;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))||checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return 0;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        if(parsePointer()) return 1;
    }
    if(checkLookAhead(TokenKind::identifier)){
        if(parseDeclarator()) return 1;
    }
    else{
        if(parseAbstractDeclarator()) return 1;
    }
    
    return 0;
}
bool c4::service::parser::LLParser::parseDeclarationSpecifier()
{
    visit();
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    if (visitor.getSepcificValue().k == Keyword::Int)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Int));
    }
    else if(visitor.getSepcificValue().k == Keyword::Void)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Void));
    }
    else if(visitor.getSepcificValue().k == Keyword::Char)
    {
        consume(TokenKind::keyword,SpecifiedToken(Keyword::Char));
    }
    else{
        //std::cout<<"Dupa\n";
        if(parseStructorUnionSpecifier()) return 1;
    }
    return 0;
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
