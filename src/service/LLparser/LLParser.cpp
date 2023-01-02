#include "LLParser.h"
using namespace c4::util::token;
//using namespace c4::util::token::Keyword;
using namespace c4::service::parser;
using namespace c4::model::token;

bool LLParser::parse(io::IBufferedInputStream<std::shared_ptr<const model::token::Token>> &input){
    std::shared_ptr<const Token> lookahead;
    m_input=input;
    bool EOFreached=false, accepting=true;
    ParserVisitor visitor;
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
bool LLParser::parseDeclaration(){
    std::shared_ptr<const Token> token;
    ParserVisitor visitor;
    //First lets parse the type specifier
    m_input.pushMark();
    m_input.read(&token);
    token->accept(visitor);
    switch(visitor.getKind()){
        case TokenKind::keyword:
        switch((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword){
        case Keyword::Void:
        case Keyword::Int:
        case Keyword::Char:
        m_input.popMark();
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
        
    } 
}
bool LLParser::parseStaticAssertDeclaration(){
    std::shared_ptr<const Token> token;
    ParserVisitor visitor;
   
    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::keyword) return 1;
    if((std::dynamic_pointer_cast<const KeywordToken>(token))->keyword!=Keyword::__Static_assert) return 1;

    m_input.read(&token);
    token->accept(visitor);
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if((std::dynamic_pointer_cast<const PunctuatorToken>(token))->punctuator!=Punctuator::LeftParenthesis) return 1;

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
    std::shared_ptr<const Token> token;
    std::shared_ptr<const Token> token1;
    ParserVisitor visitor;

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
        // TODO PARSE STRUCT DECLARATION LIST
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
        // TODO PARSE STRUCT DECLARATION LIST
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
    
}