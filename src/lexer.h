#pragma once

#include <memory>
#include <string>

#include "model/token/CharacterConstantToken.h"
#include "model/token/DecimalConstantToken.h"
#include "model/token/ErrorToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/KeywordToken.h"
#include "model/token/StringLiteralToken.h"
#include "model/token/Token.h"
#include "model/token/TokenPosition.h"
#include "service/automata/IAutomaton.h"
#include "service/io/IInputStream.h"

namespace c4 {

class Lexer {
private:
    std::shared_ptr<c4::service::io::IInputStream<char>> charStream;
    std::shared_ptr<const c4::service::automata::IAutomaton<char, c4::model::token::Token>> punctuators;
    std::shared_ptr<const c4::service::automata::IAutomaton<char, c4::model::token::Token>> keywords;

public:
    Lexer(const std::shared_ptr<c4::service::io::IInputStream<char>> charStream, 
        std::shared_ptr<c4::service::automata::IAutomaton<char, c4::model::token::Token>> punctuators,
        std::shared_ptr<c4::service::automata::IAutomaton<char, c4::model::token::Token>> keywords) : 
    charStream(charStream), punctuators(punctuators), keywords(keywords) {}
    
    bool nextToken(std::shared_ptr<const c4::model::token::Token>&);
};

}