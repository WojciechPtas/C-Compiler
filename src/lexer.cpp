#include "lexer.h"
#include <string>
#include "model/token/TokenPosition.h"
#include "model/token/KeywordToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/DecimalConstantToken.h"

// #include "model/token/"

using namespace c4::model::token;
using namespace c4::service::io; //I hope this is fine

bool isStartOfIdentifier(const char c) {
    return (isalpha(c) || c=='_');
}

bool isCharOfIdentifier(const char c) {
    return (isalnum(c) || c=='_');
}

bool isNonZeroDigit(const char c) {
    return (isdigit(c) && c!='0');
}

bool isCharOfNumberConstant(const char c, const bool isFloat, const bool exponentFound) {
    return   isdigit(c) || 
            (c=='.' && !isFloat) || 
            (tolower(c)=='e' && isFloat && !exponentFound);
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
        bool possibleInteger = isNonZeroDigit(c);
        bool validChar = true;
        bool isFloat = false;
        bool exponentFound = false;

        while(!eof_reached && validChar) {
            word.append(1, c); //Appends c to the word
            eof_reached = !charStream.read(&c); 
            validChar = isCharOfNumberConstant(c, isFloat, exponentFound);
        }

        token = new DecimalConstantToken(tp, word);
        if(!possibleInteger && !isFloat) {
            //WE NEED TO RETURN 2 TOKENS: 0 CONSTANT AND INTEGER CONSTANT!
        }
    }

    
    
    return eof_reached;
}

}