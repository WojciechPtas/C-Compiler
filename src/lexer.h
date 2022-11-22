#pragma once

#include <memory>
#include <string>

#include "model/token/Keyword.h"
#include "model/token/Punctuator.h"
#include "model/token/Token.h"
#include "service/automata/IAutomaton.h"
#include "service/io/IFileInputStream.h"
#include "model/token/CharacterConstantToken.h"
#include "model/token/DecimalConstantToken.h"
#include "model/token/ErrorToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/KeywordToken.h"
#include "model/token/PunctuatorToken.h"
#include "model/token/StringLiteralToken.h"

namespace c4 {

class Lexer {
private:
    std::shared_ptr<c4::service::io::IFileInputStream<char>> charStream;
    const std::shared_ptr<const c4::service::automata::IAutomaton<char, c4::model::token::Punctuator>> punctuators;
    const std::shared_ptr<const c4::service::automata::IAutomaton<char, c4::model::token::Keyword>> keywords;

    bool readMaximumMunchWhile(std::string& wordToAppendTo, bool (*filter) (char)); //Takes a function which returns bool on characters, reads from the stream while possible, consumes only valid chars
    bool readMaximumMunchUntil(std::string& wordToAppendTo, const std::string& terminator); //Takes a function which returns bool on characters, reads from the stream while possible, consumes only valid chars

public:
    Lexer(const std::shared_ptr<c4::service::io::IFileInputStream<char>> charStream, 
        std::shared_ptr<c4::service::automata::IAutomaton<char, c4::model::token::Punctuator>> punctuators,
        std::shared_ptr<c4::service::automata::IAutomaton<char, c4::model::token::Keyword>> keywords) : 
    charStream(charStream), punctuators(punctuators), keywords(keywords) {}
    
    bool nextToken(std::shared_ptr<const c4::model::token::Token>&);
};

}