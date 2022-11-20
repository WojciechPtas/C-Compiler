#include "lexer.h"
#include <string>
#include "model/token/TokenPosition.h"
#include "model/token/KeywordToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/DecimalConstantToken.h"
#include "model/token/CharacterConstantToken.h"

using namespace c4::model::token;
using namespace c4::service::io; //I hope this is fine

bool isStartOfIdentifier(const char c) {
    return (isalpha(c) || c=='_');
}

bool isCharOfIdentifier(const char c) {
    return (isalnum(c) || c=='_');
}

bool escapeSequence(const char c) {
    return 
        (c=='\'') || (c=='\"') || (c=='\?') || 
        (c=='\\') || (c=='a') || (c=='b') || 
        (c=='f') || (c=='n') || (c=='r') || 
        (c=='t') || (c=='v');
}

namespace c4 {

bool Lexer::nextToken(const Token* token) {
    TokenPosition tp(charStream.getFilePath(), charStream.getPosLine(), charStream.getPosColumn());
    std::string word;
    char c;
    charStream.pushMark();
    bool eof_reached = !charStream.read(&c);
    if(eof_reached) return false;
    
//Start of lexing process

    //Case: keyword or identifier
    if(isStartOfIdentifier(c)) {
        // while(!eof_reached && isCharOfIdentifier(c)) {
        //     word.append(1, c); //Appends c to the word
        //     eof_reached = !charStream.read(&c); //if it reads last char, we still expect eof_reached to be false
        // }
        charStream.resetToMark();
        //AUTOMATON TIME!
        
        //IS word IDENTIFIER OR KEYWORD??
    }

    //Case: number constants
    else if (c == '0') {
        // 0 constant
    }

    else if(isdigit(c)) { //Nonzero decimal constant
        while(!eof_reached && isdigit(c)) {
            word.append(1, c); //Appends c to the word
            eof_reached = !charStream.read(&c);
        }
        token = new DecimalConstantToken(tp, word);
    }

    //Case: char constants
    if(c=='\'') {
        bool valid = charStream.read(&c);
        // if (!valid) return errortoken;
        
        //Inside the quotes
        if (c == '\\') { //Potential escape sequence!
            //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
            valid = !charStream.read(&c) || escapeSequence(c);
            word.append(1, '\\');
        }

        else if(c=='\'' || c=='\n')
            //return errortoken;
        
        word.append(1, c);

        if (charStream.read(&c) && c=='\'') {
            //return chartoken;
        }
        else {
            // return errortoken;
        }
    }
        
        
}

}