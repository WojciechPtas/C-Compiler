#include "LLParser.h"
using namespace c4::util::token;
using namespace c4::service::parser;
using namespace c4::model::token;
// DONE!
bool LLParser::checkLookAhead(TokenKind k, SpecifiedToken s)
{
    m_input.pushMark();
    bool seen = consume(k,s);
    m_input.resetToMark();
    m_input.popMark();
    return seen;
}

//DONE!
bool LLParser::consume(TokenKind k, SpecifiedToken s={.empty=true})
{
    m_input.read(&token);
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


bool LLParser::parse(io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input){
    std::shared_ptr<const Token> lookahead;
    m_input=input;
    bool EOFreached=false, accepting=true;
    //ParserVisitor visitor;
    // First, we need to check whetver we should parse extern definition or function definition

    m_input.pushMark();
    
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
    EOFreached=!m_input.read(&lookahead);
    if(!EOFreached){
        return parse(m_input);
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
        if(this->parseDeclarator()) return 1;
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
// TODO PARSE STRUCT DECLARATION
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
// DONE!
bool c4::service::parser::LLParser::parsePointer()
{
    //Consume asterisk
    // m_input.read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 1;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 1;
    if(this->consume(TokenKind::punctuator,{.p=Punctuator::Asterisk})) return 1;
    //Check if we should parse another pointer;
    // m_input.pushMark();
    // m_input.read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()!=TokenKind::punctuator) return 0;
    // if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::Asterisk) return 0;
    // m_input.resetToMark();
    // m_input.popMark();
    if(!checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Asterisk})) return 0;
    return this->parsePointer();
}
// DONE!
bool c4::service::parser::LLParser::parseDeclarator()
{
    //std::shared_ptr<const Token> token;
    //ParserVisitor visitor;
    // m_input.pushMark();
    // m_input.read(&token);
    // token->accept(visitor);
    // if(visitor.getKind()==TokenKind::punctuator)
    // {
    //     if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator==Punctuator::Asterisk) 
    //     m_input.resetToMark();
    //     m_input.popMark();
    //     this->parsePointer();
    // }
    if(checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Asterisk})) 
    {
        if(parsePointer()) return 1;
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
        if(visitor.getSepcificValue().p!=Punctuator::LeftParenthesis) return 1;
        m_input.popMark();
        if(this->parseDeclarator()) return 1;
        if(consume(TokenKind::punctuator,{.p=Punctuator::RightParenthesis})) return 1;
        return 0;
        default:
        m_input.resetToMark();
        m_input.popMark();
        if(this->parseDirectDeclarator()) return 1;
        break;
    }
    // Now we need to decide which productions we will use
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    switch(visitor.getSepcificValue().p){
        case Punctuator::LeftParenthesis:
        // we encounterd ither parameter type list or identyfier type list
        //m_input.pushMark();
        //m_input.read(&token);
        //token->accept(visitor);
        //m_input.resetToMark();
        //m_input.popMark();
        if(!checkLookAhead(TokenKind::punctuator,{.p=Punctuator::RightBrace})){
            if(checkLookAhead(TokenKind::identifier))
            {            
                if(this->parseIdentifierList()) return 1;
            }
            else{
            if(this->parseParameterTypeList()) return 1;
            }
        }
        //parse ")"
        if(this->consume(TokenKind::punctuator,{.p=Punctuator::RightBrace})) return 1;
        return 0;
        case Punctuator::LeftBracket:
            m_input.read(&token);
            token->accept(visitor);
            switch(visitor.getKind()){
                case TokenKind::keyword:
                switch(visitor.getSepcificValue().k){
                    case Keyword::Static:
                    // parse TYPE QUALFIER LIST
                    // PARSE ASSIGNMENT EXPRESSION OPT
                    case Keyword::Void:
                    case Keyword::Int:
                    case Keyword::Char:
                    if(checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Asterisk})) {
                        consume(TokenKind::punctuator,{.p=Punctuator::Asterisk});
                        break; // We only need to parse right bracket
                    }
                }
            }
        default:
        return 1;
    }
    if(consume(TokenKind::punctuator,{.p=Punctuator::RightBracket})) return 1;
    return 0;
}
// TODO IMPLEMENT
bool c4::service::parser::LLParser::parseParameterTypeList()
{
    return false;
}
// DONE
bool c4::service::parser::LLParser::parseIdentifierList()
{
    if(consume(TokenKind::identifier)) return 1;
    if(!checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Comma})) return 0;
    if(checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Comma})) return 1;
    parseIdentifierList();
}
// DONE
bool c4::service::parser::LLParser::parseCompundStatement()
{
    if(consume(TokenKind::punctuator,{.p=Punctuator::LeftBrace})) return 1;
    while(!checkLookAhead(TokenKind::punctuator,{.p=Punctuator::RightBrace})){
        if(checkLookAhead(TokenKind::keyword,{.k=Keyword::__Static_assert})){
            if(parseStaticAssertDeclaration()) return 1;
        }
        else if(checkLookAhead(TokenKind::keyword,{.k=Keyword::Int})||checkLookAhead(TokenKind::keyword,{.k=Keyword::Void})||checkLookAhead(TokenKind::keyword,{.k=Keyword::Char})){
            if(parseDeclaration()) return 1;
        }
        else{
            if(parseStatement()) return 1;
        }
    }
    if(checkLookAhead(TokenKind::punctuator,{.p=Punctuator::RightBrace})) return 1;
    return 0;

}
// TODO PARSE EXPRESSION
bool c4::service::parser::LLParser::parseSelectionStatement()
{
    if(consume(TokenKind::keyword,{.k=Keyword::If})) return 1;
    if(consume(TokenKind::punctuator,{.p=Punctuator::LeftParenthesis})) return 1;
    // PARSE EXPRESSION
    if(consume(TokenKind::punctuator,{.p=Punctuator::RightParenthesis})) return 1;
    if(parseStatement()) return 1;
    if(!checkLookAhead(TokenKind::keyword,{.k=Keyword::Else}))return 0;
    consume(TokenKind::keyword,{.k=Keyword::Else});
    if(parseStatement()) return 1;
    return 0;
}
// TODO PARSE EXPRESSION
bool c4::service::parser::LLParser::parseIterationStatement()
{
    if(consume(TokenKind::keyword,{.k=Keyword::While})) return 1;
    if(consume(TokenKind::punctuator,{.p=Punctuator::LeftParenthesis})) return 1;
    // PARSE EXPRESSION
    if(consume(TokenKind::punctuator,{.p=Punctuator::RightParenthesis})) return 1;
    if(parseStatement()) return 1;
    return 0;

}
// DONE
bool c4::service::parser::LLParser::parseJumpStatement()
{
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    switch(std::dynamic_pointer_cast<const KeywordToken>(token)->keyword){
        case Keyword::Goto:
            if(consume(TokenKind::identifier)) return 1;
        case Keyword::Continue:
        case Keyword::Break:
        break;
        case Keyword::Return:
        if(this->checkLookAhead(TokenKind::punctuator,{.p=Punctuator::Semicolon})) break;
        else{
        // TODO PARSE EXPRESSION
        }
        break;
        default:
        return 1;
    }
    if(consume(TokenKind::punctuator, {.p=Punctuator::Semicolon})) return 1;
    return 0;
}
// TODO PARSE EXPRESSION STMT
bool c4::service::parser::LLParser::parseStatement()
{
    if(checkLookAhead(TokenKind::identifier)){
        return (parseLabeledStatement()) ? 1: 0;
    }
    else if(checkLookAhead(TokenKind::punctuator,{.p=Punctuator::LeftBrace})){
        return (parseCompundStatement()) ? 1: 0;
    }
    else if(checkLookAhead(TokenKind::keyword)){
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
    // TODO PARSE EXPRESSION STMT
}

bool c4::service::parser::LLParser::parseLabeledStatement()
{
    if(consume(TokenKind::identifier)) return 1;
    if(consume(TokenKind::punctuator,{.p=Punctuator::Colon})) return 1;
    return parseStatement();
}
