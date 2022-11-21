#include "lexer.h"
#include <iostream>

using namespace c4::model::token;
using namespace c4::service;

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

bool Lexer::nextToken(std::shared_ptr<const Token> &token) {
    token = nullptr;
    TokenPosition tp(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
    std::string word;
    char c;
    bool eof_reached, validEndOfFile=false;

    charStream->pushMark();
    while(!(eof_reached = !(charStream->read(&c))) && isspace(c))  {
        validEndOfFile = (c== '\n' || c=='\r'); //File must end in a newline!
    } //Wastes all whitespaces, newlines, etc.
    std::cout << "Last char read was " << c << "\n";
    if(eof_reached) {
        if(!validEndOfFile) {
            tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
            token = std::make_shared<ErrorToken>(tp, "Unexpected end of file");
        }
        return false;
    }
//We continue only if we didn't reach EOF

//Start of lexing process
    //Case: keyword or identifier
    if(isStartOfIdentifier(c)) {
        std::cout << "Consider identifier\n";
        charStream->resetToMark();
        // token = keywords->walk(*charStream); //After this, only accepted part of the stream has been used up
        eof_reached = !charStream->read(&c);
        std::cout << "Read a " << c << "second\n";
        if(token == nullptr || (!eof_reached && isCharOfIdentifier(c)) ) { //No keyword found OR keyword found but other letters are following
            charStream->resetToMark();
            while(charStream->read(&c) && isCharOfIdentifier(c)) {
                std::cout << "another character! " << c << '\n';
                charStream->popMark();
                charStream->pushMark();
                word.append(1, c); //Appends c to the word
            }
            charStream->resetToMark(); //makes sure stream is now after the last valid char was read
            token = std::make_shared<IdentifierToken>(tp, word);
        }
    }

    //Case: number constants
    else if (c == '0') {
        token = std::make_shared<DecimalConstantToken>(tp, "0");
    }

    else if(isdigit(c)) { //Nonzero decimal constant
        while(!eof_reached && isdigit(c)) {
            word.append(1, c); //Appends c to the word
            eof_reached = !charStream->read(&c);
        }
        token = std::make_shared<DecimalConstantToken>(tp, word);
    }

    //Case: char constants
    else if(c=='\'') {
        eof_reached = !charStream->read(&c);
        if (eof_reached) {
            tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
            token = std::make_shared<ErrorToken>(tp, "EOF in the middle of a character constant");
            return false;
        }

        //Inside the quotes
        bool valid=true;
        if (c == '\\') { //Potential escape sequence!
            //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
            word.append(1, '\\');
            bool invalidEscapeSequence = !charStream->read(&c) || !escapeSequence(c);
            if (invalidEscapeSequence) {
                tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
                token = std::make_shared<ErrorToken>(tp, "Invalid escape sequence");
                valid = false;
            }
        }

        else if(c=='\'' || c=='\n') {
            tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
            // std::string errorMsg = "Disallowed character ";
            // errorMsg.append(1, c); errorMsg.append(" in character constant")
            token = std::make_shared<ErrorToken>(tp, "Disallowed character in character constant");
            valid = false;
        }
        word.append(1, c); //append last character read before continuing
        if (valid && charStream->read(&c) && c=='\'') {
            token = std::make_shared<CharacterConstantToken>(tp, word);
        }
        else if (/*it WAS*/ valid) { //we don't wanna overwrite error messages
            tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
            token = std::make_shared<ErrorToken>(tp, "Expected ' to terminate the character constant");
        }
        //else i already have the errortoken ready
    }

    //Case: string
    else if (c=='\"') {
        bool stringTerminated = false, valid = true;
        while(charStream->read(&c) && valid && !stringTerminated) {
            if (c== '\"') { //String terminated correctly
                token = std::make_shared<StringLiteralToken>(tp, word);
                stringTerminated = true;
            }
            else if (c == '\\') { //Potential escape sequence!
                //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
                word.append(1, '\\');
                bool invalidEscapeSequence = !charStream->read(&c) || !escapeSequence(c);
                if (invalidEscapeSequence) {
                    tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
                    token = std::make_shared<ErrorToken>(tp, "Invalid escape sequence");
                    valid = false;
                }
            }
            else if (c== '\n') {
                valid = false;
                tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
                token = std::make_shared<ErrorToken>(tp, "Newlines in string literals are not allowed");
            }

            if(!stringTerminated) word.append(1, c);
        }
        if (stringTerminated) {
            token = std::make_shared<StringLiteralToken>(tp, word);
        }
    }

    else {
        std::cout << "Nothing found\n";
        charStream->resetToMark();
        token = punctuators->walk(*charStream);
        if (token == nullptr) {
            tp = TokenPosition(charStream->getSourceName(), charStream->getPosLine(), charStream->getPosColumn());
            token = std::make_shared<ErrorToken>(tp, "Unrecognized symbol");
        }
        //else punctuator token is ready by the automaton
    }
        
    charStream->popMark();
    return true; //even if we reached eof, we want to say it in the next iteration of nextToken() so we can report EOF without a previous \n

}

}