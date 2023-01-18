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
// DONE!
bool LLParser::checkLookAhead(TokenKind k, SpecifiedToken s)
{
    std::cout<<"In lookahead:";
    m_input->pushMark();
    bool seen = !consume(k,s,true);
    m_input->resetToMark();
    m_input->popMark();
    return seen;
}

//DONE!
bool LLParser::consume(TokenKind k, SpecifiedToken s, bool inlookahead)
{
    if(!inlookahead) std::cout<<"Consumed:";    
    auto a = m_input->read(&token);
    if(!a) return 1;
    token->accept(visitor);
    if(visitor.getKind()!=k) return 1;
    switch(k){
        case TokenKind::keyword:
        return !(visitor.getSepcificValue().k==s.k);
        case TokenKind::punctuator:
        return !(visitor.getSepcificValue().p==s.p);
        default:
        return 0;
    }
}


bool LLParser::parse(/*io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input*/){
    //std::unique_ptr<Token> lookahead;
    //m_input=input;
    //std::cout << " parsing \n";
std::cout << "parse():\n";
    //bool EOFreached=false;
    //ParserVisitor visitor;
    // First, we need to check whetver we should parse extern definition or function definition

    visit();
    //std::cout << " parsing \n";
    switch(visitor.getKind()){
        case TokenKind::keyword:
        switch(visitor.getSepcificValue().k){
            case Keyword::__Static_assert:
            if(this->parseStaticAssertDeclaration()) return 1;
            break;
            default:
            if(this->parseDeclaration()) {
                std::cout<<"Exiting with 1\n";
                return 1;
            }
        }
        break;
        default:
        return 1;
    }
    std::cout<<"Keep going!\n";
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
        if(parseCompoundStatement()) return 1;
    }
    //m_input->pushMark();
    //bool a = m_input->read(&token);
    //m_input->resetToMark();
    //m_input->popMark();
    m_input->pushMark();
    auto a = m_input->read(&token);
    m_input->resetAndPopMark();
    if(a) return parse();
    return 0;

}

bool LLParser::parseDeclaration()
{

    //First lets parse the type specifier
    std::cout << "parseDeclaration()\n";

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
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) {
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon)); // TODO CC
            return 0;
        }
        return 0;
    }
    //m_input->pushMark();
    //m_input->read(&token);
//
    //std::cout<<"Consumed:";
    //token->accept(visitor);
    //switch(visitor.getKind())
    //{
    //    case TokenKind::keyword:
    //    switch((visitor.getSepcificValue().k)){
    //    case Keyword::Void:
    //    case Keyword::Int:
    //    case Keyword::Char:
    //    //std::cout << " VOID! \n";
    //    m_input->popMark();
    //    if(this->parseDeclarator()) {
    //        std::cout<<"Exiting parseDeclaration with 1\n";
    //        return 1;
    //    }
    //    if(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return 0;
    //    break;
    //    case Keyword::Struct:
    //    case Keyword::Union:
    //    m_input->resetToMark();
    //    m_input->popMark();
    //    if(this->parseStructorUnionSpecifier()) return 1;
    //    break;
    //    default:
    //    return 1;
    //    }
    //    break;
    //    default:
    //    return 1;
    //}
    //if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))){
    //    std::cout<<"Exiting parseDeclaration with 1\n";
    //    return 1;
    //}
    //return 0;
}

bool LLParser::parseStaticAssertDeclaration()
{
    //std::shared_ptr<const Token> token;
    ////ParserVisitor visitor;
   
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::keyword) return 1;
    // if((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword!=Keyword::__Static_assert) return 1;
    if(this->consume(TokenKind::keyword,SpecifiedToken(Keyword::__Static_assert))) return 1;

    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 1;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftParenthesis) return 1;
    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
    // @TODO INVOKE LR PARSER HERE TO PARSE CONSTANT EXPRESSION

    //m_input->read(&token);
    //token->accept(visitor);
    //if(visitor.getKind()!=TokenKind::punctuator) return 1;
    //if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Comma) return 1;

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return 1;

    //m_input->read(&token);
    //token->accept(visitor);
    //if(visitor.getKind()!=TokenKind::string_literal) return 1;

    if(this->consume(TokenKind::string_literal)) return 1;

    //m_input->read(&token);
    //token->accept(visitor);
    //if(visitor.getKind()!=TokenKind::punctuator) return 1;
    //if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::RightParenthesis) return 1;

    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;

    //m_input->read(&token);
    //token->accept(visitor);
    //if(visitor.getKind()!=TokenKind::punctuator) return 1;
    //if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Semicolon) return 1;
    
    if(this->consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon))) return 1;


    return 0;
}

bool LLParser::parseStructorUnionSpecifier(){
    //std::shared_ptr<const Token> token;
    //std::shared_ptr<const Token> token1;
    ////ParserVisitor visitor;

    m_input->read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    if(visitor.getSepcificValue().k!=Keyword::Struct || visitor.getSepcificValue().k!=Keyword::Union) return 1;

    
    m_input->read(&token);
    token->accept(visitor);
    if(visitor.getKind()==TokenKind::identifier){
        m_input->pushMark();
        m_input->read(&token);
        token->accept(visitor);
        if(visitor.getKind()==TokenKind::punctuator){ 
        if(visitor.getSepcificValue().p!=Punctuator::LeftBrace) {
             m_input->popMark();
            return 0;
        }
        if(this->parseStructDeclarationList()) return 1;
        m_input->read(&token);
        token->accept(visitor);
        if(visitor.getKind()!=TokenKind::punctuator) return 1;
        if(visitor.getSepcificValue().p!=Punctuator::RightBrace) return 1;
        m_input->popMark();
        return 0;
    }
    }
    else if(visitor.getKind()==TokenKind::punctuator){ 
        if(visitor.getSepcificValue().p!=Punctuator::LeftBrace) return 1;
        m_input->popMark();
        if(this->parseStructDeclarationList()) return 1;
        m_input->read(&token);
        token->accept(visitor);
        if(visitor.getKind()!=TokenKind::punctuator) return 1;
        if(visitor.getSepcificValue().p!=Punctuator::RightBrace) return 1;
        m_input->popMark();
        return 0;
    }
   return 1;
}
// TODO PARSE STRUCT DECLARATION
bool LLParser::parseStructDeclarationList(){
    //ParserVisitor visitor;

    // TODO PARSE STRUCT DECLARATION
    //std::shared_ptr<const Token> token;
    m_input->pushMark();
    m_input->read(&token);
    token->accept(visitor);
    if(visitor.getKind()==TokenKind::punctuator){
        if(visitor.getSepcificValue().p==Punctuator::RightBrace){
            m_input->resetToMark();
            m_input->popMark();
            return 0;
        }
        return 1;
    }
    else{
        m_input->resetToMark();
        m_input->popMark();
         return this->parseStructDeclarationList();
    }
    return 1;
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
    //std::shared_ptr<const Token> token;
    //ParserVisitor visitor;
    // m_input->pushMark();
    // m_input->read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()==TokenKind::punctuator)
    // {
    //     if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator==Punctuator::Asterisk) 
    //     m_input->resetToMark();
    //     m_input->popMark();
    //     this->parsePointer();
    // }
    std::cout<< "parseDeclarator()\n";
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))) 
    {
        if(parsePointer()) return 1;
    }
    //std::cout<< "Direct declarator\n";
    return parseDirectDeclarator();
}

bool c4::service::parser::LLParser::parseDirectDeclarator()
{
    std::cout<< "parseDirectDeclarator()\n";
    visit();
    if(visitor.getKind()==TokenKind::identifier){
        if(consume(TokenKind::identifier)) return 1;
    }
    else{
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
        parseDeclarator();
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    }
   return parseDirectDeclarator2(); 
}
bool c4::service::parser::LLParser::parseDirectDeclarator2()
{
    visit();
    if(visitor.getKind()==TokenKind::punctuator && visitor.getSepcificValue().p==Punctuator::LeftParenthesis){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis));
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis)))
        {
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis));
        }
        else if(visitor.getKind()==TokenKind::identifier){
            if(parseIdentifierList()) return 1;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
        }
        else{
            if(parseParameterTypeList()) return 1;
            if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
        }
    }
    else if(visitor.getKind()==TokenKind::punctuator && visitor.getSepcificValue().p==Punctuator::LeftBracket){
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBracket));
        if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket));
        }
        else if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Static))){
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Static));
            visit();
            if(visitor.getKind()==TokenKind::keyword){
                if(visitor.getSepcificValue().k==Keyword::Void || visitor.getSepcificValue().k==Keyword::Int || visitor.getSepcificValue().k==Keyword::Char ||visitor.getSepcificValue().k==Keyword::Struct)
                {
                    if(parseDeclarationSpecifier()) return 1;
                }
                // TODO PARSE ASSIGNMENT EXPR
            }
        }
        else if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))){
            consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk));
        }
        else if(visitor.getKind()==TokenKind::punctuator){
            // TODO PARSE ASSIGNMENT EXPR
        }
        else{
            if(parseDeclarationSpecifier()) return 1;
            if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Static))){
                consume(TokenKind::keyword,SpecifiedToken(Keyword::Static));
                // TODO PARSE ASSIGNMENT EXPR
            }
            else if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk))){
                consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Asterisk));
            }
            else{
                // TODO PARSE ASSIGNMENT EXPR
            }
        }
        if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBracket))) return 1;
    }
    return 0;
}
// DONE
bool c4::service::parser::LLParser::parseParameterTypeList()
{
    if(parseParameterDeclaration()) return 1;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma)))  {
        consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma));
        parseParameterTypeList();
    }
    return 0;
}
bool c4::service::parser::LLParser::parseParameterDeclaration()
{
    if(parseDeclarationSpecifier()) return 1;
    // TODO PARSE ABSTRACT DECLARATOR
    if(parseDeclarator()) return 1;
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
        if(parseStructorUnionSpecifier()) return 1;
    }
    return 0;
}
// DONE
bool c4::service::parser::LLParser::parseIdentifierList()
{
    std::cout<<"parseIdentifierList()\n";
    if(consume(TokenKind::identifier)) return 1;
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::Comma))) return parseIdentifierList();
    return 0;
}
// DONE
bool c4::service::parser::LLParser::parseCompoundStatement() // {dasdasd}
{
    std::cout<<"parseCompoundStatement()\n";
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))) return 1;
    while(!checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))){
        if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::__Static_assert))){
            if(parseStaticAssertDeclaration()) return 1;
        }
        else if(checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Int))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Void))||checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Char))){
            std::cout<<"parsing declaration\n";
            
            if(parseDeclaration()) return 1;
            std::cout<<"parsed declaration\n";
            //if(parseStatement()) return 1;
        }
        else{
            std::cout<<"parsing statement\n";
            if(parseStatement()) return 1;
            //exit(1);
        }
    }
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightBrace))) return 1;
    return 0;

}
// TODO PARSE EXPRESSION
bool c4::service::parser::LLParser::parseSelectionStatement()
{
    std::cout << "parseSelectionStatement()\n";
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::If))) return 1;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
    ParenthesisDelimiterStream stream(m_input);
    //auto a = PrintVisitor(std::cout);
    auto a=std::make_shared<State>(INITIAL_STATE);
    auto lrparser = std::make_shared<ExpressionParser>(a);
    lrparser->parse(stream);//->accept(a);
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    if(parseStatement()) return 1;
    if(!checkLookAhead(TokenKind::keyword,SpecifiedToken(Keyword::Else)))return 0;
    consume(TokenKind::keyword,SpecifiedToken(Keyword::Else));
    if(parseStatement()) return 1;
    return 0;
}
// TODO PARSE EXPRESSION
bool c4::service::parser::LLParser::parseIterationStatement()
{
    if(consume(TokenKind::keyword,SpecifiedToken(Keyword::While))) return 1;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftParenthesis))) return 1;
    // PARSE EXPRESSION
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::RightParenthesis))) return 1;
    if(parseStatement()) return 1;
    return 0;

}
// DONE
bool c4::service::parser::LLParser::parseJumpStatement()
{
    std::cout<<"parseJumpStatement()\n";
    visit();
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    switch(visitor.getSepcificValue().k){
        case Keyword::Goto:
            consume(TokenKind::keyword,SpecifiedToken(Keyword::Goto));
            if(consume(TokenKind::identifier)) return 1;
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
        // TODO PARSE EXPRESSION
        }
        break;
        default:
        return 1;
    }
    if(consume(TokenKind::punctuator, SpecifiedToken(Punctuator::Semicolon))) return 1;
    return 0;
}
// TODO PARSE EXPRESSION STMT
bool c4::service::parser::LLParser::parseStatement()
{
    std::cout<<"parseStatement()\n";
    visit();
    if(visitor.getKind()==(TokenKind::identifier)){
        std::cout<<"dupa1!\n";
        m_input->pushMark();
        m_input->read(&token);
        visit();
        m_input->resetAndPopMark(); // LL2
        if(visitor.getKind()==TokenKind::punctuator){
            if(visitor.getSepcificValue().p==Punctuator::Colon){
                std::cout << "dupa2\n";
                return (parseLabeledStatement()) ? 1: 0;
            }
        }
    }
    if(checkLookAhead(TokenKind::punctuator,SpecifiedToken(Punctuator::LeftBrace))){
        std::cout<<"dupa2\n";

        return (parseCompoundStatement()) ? 1: 0;
    }
    else if(visitor.getKind()==(TokenKind::keyword)){
        switch(visitor.getSepcificValue().k){
            case Keyword::If:
            return parseSelectionStatement() ? 1 : 0;
            case Keyword::While:
            return parseIterationStatement() ? 1 : 0;
            case Keyword::Goto:
            case Keyword::Continue:
            case Keyword::Return:
            return parseJumpStatement() ? 1 : 0;
            default:
            break;
        }
    }
    DelimiterStream stream(m_input,TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    //auto a = PrintVisitor(std::cout);
    auto a=std::make_shared<State>(INITIAL_STATE);
    auto lrparser = std::make_shared<ExpressionParser>(a);
    lrparser->parse(stream);//->accept(a);
    //}
    //catch (std::logic_error&){};
    return consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
}

bool c4::service::parser::LLParser::parseLabeledStatement()
{
    if(consume(TokenKind::identifier)) return 1;
    if(consume(TokenKind::punctuator,SpecifiedToken(Punctuator::Colon))) return 1;
    return parseStatement();
}

bool c4::service::parser::LLParser::visit()
{
    checkLookAhead(TokenKind::identifier);
    return false;
}
