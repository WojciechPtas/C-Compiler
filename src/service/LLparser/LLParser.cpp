#include "LLParser.h"
using namespace c4::util::token;
//using namespace c4::util::token::Keyword;
using namespace c4::service::parser;
using namespace c4::model::token;

bool LLParser::parse(io::IInputStream<std::shared_ptr<const model::token::Token>> &input){
    std::shared_ptr<const Token> lookahead;
    m_input=input;
    bool EOFreached=false, accepting=true;
    ParserVisitor visitor;
    // First, we need to check whetver we should parse extern definition or function definition
    m_input.pushMark();
    EOFreached=!m_input.read(&lookahead);
    visitor.visit(lookahead);
    switch(visitor.getKind()){
        case TokenKind::keyword:
        switch(dynamic_cast<KeywordToken>(lookahead).getKeyword()){
            case Keyword::__Static_assert:
            m_input.resetToMark();
            m_input.popMark();
            this->parseStaticAssertDeclaration();
            break;
            default:
            m_input.resetToMark();
            m_input.popMark();
            this->parseDeclaration();
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
    visitor(visit(token));
    switch(visitor.getKind()){
        case TokenKind::keyword:
        switch(dynamic_cast<KeywordToken>(token).getKeyword()){
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
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::Keyword) return 1;
    if(dynamic_cast<KeywordToken>(token).getKeyword()!=Keyword::__Static_assert) return 1;

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if(dynamic_cast<PunctuatorToken>(token).getPunctuator()!=Punctuator::LeftParenthesis) return 1;

    // @TODO INVOKE LR PARSER HERE TO PARSE CONSTANT EXPRESSION

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if(dynamic_cast<PunctuatorToken>(token).getPunctuator()!=Punctuator::Comma) return 1;

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::string_literal) return 1;

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if(dynamic_cast<PunctuatorToken>(token).getPunctuator()!=Punctuator::RightParenthesis) return 1;

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::punctuator) return 1;
    if(dynamic_cast<PunctuatorToken>(token).getPunctuator()!=Punctuator::Semicolon) return 1;
    
    return 0;
}

bool LLParser::parseStructorUnionSpecifier(){
    std::shared_ptr<const Token> token;
    ParserVisitor visitor;

    m_input.read(&token);
    visitor(visit(token));
    if(visitor.getKind()!=TokenKind::Keyword) return 1;
    if(dynamic_cast<KeywordToken>(token).getKeyword()!=Keyword::Struct || dynamic_cast<KeywordToken>(token).getKeyword()!=Keyword::Union) return 1;

    m_input.read(&token);
    if(visitor.getKind()==TokenKind::punctuator){ 
        if(dynamic_cast<PunctuatorToken>(token).getPunctuator()!=Punctuator::LeftBracket) return 1;

    }
}