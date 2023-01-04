#include "LLParser.h"
using namespace c4::util::token;
//using namespace c4::util::token::Keyword;
using namespace c4::service::parser;
using namespace c4::model::token;

bool LLParser::checkLookAhead(TokenKind k, SpecifiedToken s)
{
    m_input.pushMark();
    bool seen = consume(k,s);
    m_input.resetToMark();
    m_input.popMark();
    return seen;
}


bool LLParser::consume(TokenKind k, SpecifiedToken s)
{
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=k) return 1;
    switch(k){
        case TokenKind::keyword:
        return !(((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword)==s.k);
        case TokenKind::punctuator:
        return !(((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator)==s.p);
        default:
        return 0;
    }
}


bool LLParser::parse(io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input){
    std::shared_ptr<const Token> lookahead;
    m_input=input;
    bool EOFreached=false, accepting=true;
    //ParserVisitor visitor;
    // First, we need to check whetver we should parse extern definition or function definition

    m_input.pushMark();
    EOFreached=!m_input.read(&lookahead);
    lookahead->accept(visitor);

    switch(visitor.getKind()){
        case TokenKind::keyword:
        switch((std::dynamic_pointer_cast<const KeywordToken>(lookahead))->keyword){
            case Keyword::__Static_assert:
            m_input.resetToMark();
            m_input.popMark();
            if(this->parseStaticAssertDeclaration()) return 1;
            break;
            default:
            m_input.resetToMark();
            m_input.popMark();
            if(this->parseDeclaration()) return 1;
        }
        break;
        default:
        return 1;
    }
}

bool LLParser::parseDeclaration()
{
    //std::shared_ptr<const Token> token;
    ////ParserVisitor visitor;
    //First lets parse the type specifier
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    switch(visitor.getKind())
    {
        case TokenKind::keyword:
        switch((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword){
        case Keyword::Void:
        case Keyword::Int:
        case Keyword::Char:
        m_input.popMark();
        // TODO PARSE DECLARATOR
        break;
        case Keyword::Struct:
        case Keyword::Union:
        m_input.resetToMark();
        m_input.popMark();
        if(this->parseStructorUnionSpecifier()) return 1;
        break;
        default:
        return 1;
        }
        default:
        return 1;
    }
    return 0;
}

bool LLParser::parseStaticAssertDeclaration()
{
    //std::shared_ptr<const Token> token;
    ////ParserVisitor visitor;
   
    // m_input.read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::keyword) return 1;
    // if((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword!=Keyword::__Static_assert) return 1;
    if(this->consume(TokenKind::keyword,{.k=Keyword::__Static_assert})) return 1;

    // m_input.read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 1;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftParenthesis) return 1;
    if(this->consume(TokenKind::punctuator,{.p=Punctuator::LeftParenthesis})) return 1;
    // @TODO INVOKE LR PARSER HERE TO PARSE CONSTANT EXPRESSION

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Comma) return 1;

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::string_literal) return 1;

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::RightParenthesis) return 1;

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Semicolon) return 1;
    
    return 0;
}

bool LLParser::parseStructorUnionSpecifier(){
    //std::shared_ptr<const Token> token;
    //std::shared_ptr<const Token> token1;
    ////ParserVisitor visitor;

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    if((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword!=Keyword::Struct || std::dynamic_pointer_cast<const KeywordToken>(token)->keyword!=Keyword::Union) return 1;

    
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()==TokenKind::identifier){
        m_input.pushMark();
        m_input.read(&token);
        token->accept(visitor);
        if(visitor.getKind()==TokenKind::punctuator){ 
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftBrace) {
             m_input.popMark();
            return 0;
        }
        if(this->parseStructDeclarationList()) return 1;
        m_input.read(&token);
        token->accept(visitor);
        if(visitor.getKind()!=TokenKind::punctuator) return 1;
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::RightBrace) return 1;
        m_input.popMark();
        return 0;
    }
    }
    else if(visitor.getKind()==TokenKind::punctuator){ 
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftBrace) return 1;
        m_input.popMark();
        if(this->parseStructDeclarationList()) return 1;
        m_input.read(&token);
        token->accept(visitor);
        if(visitor.getKind()!=TokenKind::punctuator) return 1;
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::RightBrace) return 1;
        m_input.popMark();
        return 0;
    }
    else return 1;
}

bool LLParser::parseStructDeclarationList(){
    //ParserVisitor visitor;

    // TODO PARSE STRUCT DECLARATION
    //std::shared_ptr<const Token> token;
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()==TokenKind::punctuator){
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator==Punctuator::RightBrace){
            m_input.resetToMark();
            m_input.popMark();
            return 0;
        }
        return 1;
    }
    else{
        m_input.resetToMark();
        m_input.popMark();
        this->parseStructDeclarationList();
    }

}

bool c4::service::parser::LLParser::parsePointer()
{
    //std::shared_ptr<const Token> token;
    //ParserVisitor visitor;
    //Consume asterisk
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 1;
    //Check if we should parse another pointer;
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 0;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 0;
    m_input.resetToMark();
    m_input.popMark();
    return this->parsePointer();
}

bool c4::service::parser::LLParser::parseDeclarator()
{
    //std::shared_ptr<const Token> token;
    //ParserVisitor visitor;
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()==TokenKind::punctuator)
    {
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator==Punctuator::Asterisk) 
        m_input.resetToMark();
        m_input.popMark();
        this->parsePointer();
    }
    return parseDirectDeclarator();
}

bool c4::service::parser::LLParser::parseDirectDeclarator()
{
    //std::shared_ptr<const Token> token;
    //ParserVisitor visitor;
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    switch(visitor.getKind()){
        case TokenKind::identifier:
        m_input.popMark();
        return 0;
        case TokenKind::punctuator:
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftParenthesis) return 1;
        m_input.popMark();
        this->parseDeclarator();
        m_input.read(&token);
        token->accept(visitor);
        if(visitor.getKind()!=TokenKind::punctuator) return 1;
        if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::RightParenthesis) return 1;
        return 0;
        default:
        m_input.resetToMark();
        m_input.popMark();
        this->parseDirectDeclarator();
        break;
    }
    // Now we need to decide which productions we will use
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    switch((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator){
        case Punctuator::LeftParenthesis:
        // we encounterd ither parameter type list or identyfier type list
        m_input.pushMark();
        m_input.read(&token);
        token->accept(visitor);
        m_input.resetToMark();
        m_input.popMark();
        if(visitor.getKind()==TokenKind::identifier){            
            if(this->parseIdentifierList()) return 1;
        }
        else{
            if(this->parseParameterTypeList()) return 1;
        }
        //parse ")"
        m_input.read(&token);
        token->accept(visitor);
        break;
        default:
        // TODO PARSE [ sdadasd ]
        return 1;
    }
    //
    return 0;
}

bool c4::service::parser::LLParser::parseParameterTypeList()
{
    return false;
}

bool c4::service::parser::LLParser::parseIdentifierList()
{
    return false;
}
