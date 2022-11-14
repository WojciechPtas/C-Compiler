#include "lexer.h"
#include <string>
#include "model/token/TokenPosition.h"
#include "model/token/KeywordToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/DecimalConstantToken.h"
#include "model/token/CharacterConstantToken.h"


// #include "model/token/"

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
        (c=='\\') || (c=='\a') || (c=='\b') || 
        (c=='\f') || (c=='\n') || (c=='\r') || 
        (c=='\t') || (c=='\v');
}

namespace c4 {

bool Lexer::nextToken(const Token* token) {
    TokenPosition tp(charStream.getFilePath(), charStream.getPosLine(), charStream.getPosColumn());
    std::string word;
    char c;
    bool eof_reached = !charStream.read(&c);
    if(eof_reached) return false;
    
//Start of lexing process

    //Case: keyword or identifier
    if(isStartOfIdentifier(c)) {
        while(!eof_reached && isCharOfIdentifier(c)) {
            word.append(1, c); //Appends c to the word
            eof_reached = !charStream.read(&c); //if it reads last char, we still expect eof_reached to be false
        }
        
        //IS word IDENTIFIER OR KEYWORD??
        //TREE STRUCTURE SHOULD RETURN CORRECT TOKEN OR NOTHING
        if (token == NULL) {
            token = new IdentifierToken(tp, word);
        }

    }

    //Case: number constants
    if(isdigit(c)) {
        while(!eof_reached && isdigit(c)) {
            word.append(1, c); //Appends c to the word
            eof_reached = !charStream.read(&c);
        }
        
        token = new DecimalConstantToken(tp, word);
    }

    //Case: char constants
    if(c=='\'') {
        bool valid = charStream.read(&c);
        //Inside the quotes
        if (valid) { 
            word.append(1, c);
            if (c == '\\') { //Potential escape sequence!
                //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
                valid = !charStream.read(&c) || escapeSequence(c);
                word.append(1, c);
            }

            else if(c=='\'' || c=='\n')
                valid = false;
            //Every other character is ok 
        }

        //It must now terminate with quotes
        if(valid) {
            valid = charStream.read(&c) && c=='\'';
        }

        //Time to get the token. Sadly i cannot avoid all these if(valid) because i need to preserve the position in the stream 
        if (valid) {
            token = new CharacterConstantToken(tp, word);
        }
        else {
            tp = TokenPosition(charStream.getFilePath(), charStream.getPosLine(), charStream.getPosColumn());
            //token = new ErrorToken(tp, word)
        }
        
        
    }

    
    
    return eof_reached;
}

}