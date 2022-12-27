#include <iostream>

#include "debug.h"
#include "lexer.h"

using namespace c4::model::token;
using namespace c4::service;

bool isStartOfIdentifier(const char c) {
    return (isalpha(c) || c=='_');
}

bool isCharOfIdentifier(const char c) {
    return (isalnum(c) || c=='_');
}

bool isDigit(char c) {
    return isdigit(c);
}

bool escapeSequence(const char c) {
    return 
        (c=='\'') || (c=='\"') || (c=='\?') || 
        (c=='\\') || (c=='a') || (c=='b') || 
        (c=='f') || (c=='n') || (c=='r') || 
        (c=='t') || (c=='v');
}

std::shared_ptr<TokenPosition> makeTokenPosition(
    std::shared_ptr<c4::service::io::IFileInputStream<char>> stream
) {
    return std::make_shared<TokenPosition>(
        stream->getFilePath(),
        stream->getCurrentLine(),
        stream->getCurrentColumn()
    );
}

std::shared_ptr<TokenPosition> positionOfLastChar(
    std::shared_ptr<c4::service::io::IFileInputStream<char>> stream
) {
    return std::make_shared<TokenPosition>(
        stream->getFilePath(),
        stream->getLastReadLine(),
        stream->getLastReadColumn()
    );
}

namespace c4 {

bool Lexer::readMaximumMunchWhile(std::string& wordToAppendTo, bool (*filter) (char)) {
    char c;
    bool eof_NOT_reached;
    charStream->pushMark();
    while( (eof_NOT_reached = charStream->read(&c)) && filter(c)) {
        charStream->popMark();
        charStream->pushMark();
        wordToAppendTo.append(1, c); //Appends c to the word
    }
    charStream->resetToMark(); //makes sure stream is now after the last valid char was read
    charStream->popMark();
    return eof_NOT_reached;
}

bool Lexer::readMaximumMunchUntil(std::string& wordToAppendTo, const std::string& terminator) {
    char c;
    size_t foundCount=0;
    while( foundCount<terminator.size() && charStream->read(&c) ) {
        if (terminator[foundCount] == c) {
            foundCount++;
        }
        else { //Whatever i thought was the beginning of a terminator, i need to flush into the wordToAppendTo
            wordToAppendTo.append(terminator.substr(0, foundCount));
            foundCount=0;

            wordToAppendTo.append(1, c); //Appends c to the word
        }
    }
    return (foundCount == terminator.size());
}

bool Lexer::nextToken(std::shared_ptr<const Token> &token) {
    token = nullptr;
    std::string word;
    char c;
    bool eof_reached, validEndOfFile=false;

    charStream->pushMark();
    while(!(eof_reached = !(charStream->read(&c))) && isspace(c))  {        
        //We don't want to come back to what we wasted
        charStream->popMark();
        charStream->pushMark();
    } //Wastes all whitespaces, newlines, etc.
    validEndOfFile = (c== '\n' || c=='\r'); //File must end in a newline!
    auto tp = positionOfLastChar(charStream); //Position for working tokens
    
    if(eof_reached) {
        if(!validEndOfFile) {
            token = std::make_shared<ErrorToken>(*tp, "Unexpected end of file");
        }
        charStream->popMark();
        return false;
    }

//We continue only if we didn't reach EOF

//Start of lexing process
    bool validToken=true;
    //Case: keyword or identifier
    if(isStartOfIdentifier(c)) {
        charStream->resetToMark();
        auto keyword = keywords->walk(*charStream)->getResult();

        if (keyword != nullptr) {
            token = std::make_shared<KeywordToken>(*tp, *keyword);
        }
        // token = keywords->walk(*charStream); //After this, only accepted part of the stream has been used up
        eof_reached = !charStream->read(&c);
        if(token == nullptr || (!eof_reached && isCharOfIdentifier(c)) ) { //No keyword found OR keyword found but other letters are following
            charStream->resetToMark();
            readMaximumMunchWhile(word, isCharOfIdentifier);
            token = std::make_shared<IdentifierToken>(*tp, word);
        }
    }

    //Case: number constants
    else if (c == '0') {
        token = std::make_shared<DecimalConstantToken>(*tp, "0");
    }

    else if(isdigit(c)) { //Nonzero decimal constant
        word.append(1,c);
        readMaximumMunchWhile(word, isDigit);
        token = std::make_shared<DecimalConstantToken>(*tp, word);
    }

    //Case: char constants
    else if(c=='\'') {
        eof_reached = !charStream->read(&c);
        if (eof_reached) {
            tp = positionOfLastChar(charStream);
            token = std::make_shared<ErrorToken>(*tp, "EOF in the middle of a character constant");
            validToken = false;
        }

        //Inside the quotes
        if (validToken && c == '\\') { //Potential escape sequence!
            //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
            word.append(1, '\\');
            validToken = charStream->read(&c) && escapeSequence(c);
            if (!validToken) {
                tp = positionOfLastChar(charStream);
                token = std::make_shared<ErrorToken>(*tp, "Invalid escape sequence");
                validToken = false;
            }
        }
        
        //Disallowed characters
        else if(validToken && (c=='\'' || c=='\n')) {
            tp = positionOfLastChar(charStream);
            // std::string errorMsg = "Disallowed character ";
            // errorMsg.append(1, c); errorMsg.append(" in character constant")
            token = std::make_shared<ErrorToken>(*tp, "Disallowed character in character constant");
            validToken = false;
        }
        //Everything else is fine

        word.append(1, c); //append last character read before continuing, even in case of error, y not
        if (validToken && charStream->read(&c) && c=='\'') {
            token = std::make_shared<CharacterConstantToken>(*tp, word);
        }
        else if (/*it WAS a*/ validToken) { //we don't wanna overwrite error messages
            tp = positionOfLastChar(charStream);
            token = std::make_shared<ErrorToken>(*tp, "Expected ' to terminate the character constant");
            validToken = false;
        }
        //else i already have the errortoken ready
    }

    //Case: string
    else if (c=='\"') {
        DBGOUT("lexer", "Encountered string start");

        bool stringTerminated = false;
        while(!stringTerminated && charStream->read(&c) && validToken) {
            if (c== '\"') { //String terminated correctly
                token = std::make_shared<StringLiteralToken>(*tp, word);
                stringTerminated = true;
            }
            else if (c == '\\') { //Potential escape sequence!
                //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
                word.append(1, '\\');
                bool invalidEscapeSequence = !charStream->read(&c) || !escapeSequence(c);
                if (invalidEscapeSequence) {
                    tp = positionOfLastChar(charStream);
                    token = std::make_shared<ErrorToken>(*tp, "Invalid escape sequence");
                    validToken = false;
                }
            }
            else if (c== '\n') {
                validToken = false;
                tp = positionOfLastChar(charStream);
                token = std::make_shared<ErrorToken>(*tp, "Newlines in string literals are not allowed");
                validToken = false;
            }

            if(!stringTerminated) word.append(1, c);
        }

        DBGOUT_E("lexer", "String terminated (string = %s)", word.c_str());

        if (stringTerminated) {
            token = std::make_shared<StringLiteralToken>(*tp, word);
        }
    }

    else {
        if(c == '/') {
        eof_reached = !charStream->read(&c);
            if (!eof_reached) {
                if(c == '/') { //Single-line comment
                    eof_reached = !readMaximumMunchUntil(word, "\n");
                }
                else if( c== '*') { //Multiline comment
                    eof_reached = !readMaximumMunchUntil(word, "*/");
                }
                
                if(eof_reached) {
                    tp = positionOfLastChar(charStream);
                    token = std::make_shared<ErrorToken>(*tp, "Unterminated comment :(");
                    validToken = false;
                }
                else {
                    return nextToken(token); //We don't report any comment token. We keep going
                }
            }
        }
        else { //last chance: Punctuator
            charStream->resetToMark();
            auto punctutator = this->punctuators->walk(*charStream)->getResult();

            if (punctutator != nullptr) {
                tp = makeTokenPosition(charStream);
                token = std::make_shared<PunctuatorToken>(*tp, *punctutator);
            } 
            else {
                charStream->read(&c); //Wastes the unrecognized char!
                tp = makeTokenPosition(charStream);
                token = std::make_shared<ErrorToken>(*tp, "Unrecognized symbol");
                validToken = false;
            }
        }
    }
        
    charStream->popMark();
    return validToken; //even if we reached eof, we want to say "true" so we can return the token for now. In the next iteration we'll terminate correctly or report source file not terminated by \n, which is not allowed.
    //if you substitute true to validToken it will return false only in case of EOF
}

}