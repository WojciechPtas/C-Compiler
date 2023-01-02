#include <iostream>
#include "debug.h"
#include "lexer.h"

#define CASE_ERRORTOKEN_RETURNS_TRUE false
//if set to true, nextToken() will return false only in case of EOF

using namespace c4::model;
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

bool Lexer::readEliding(char *c) { //Read wrapper that implements backslash+newline eliding
    bool backSlashNewLine = true;
    bool notEOF;
    charStream->pushMark();
    while(backSlashNewLine && (notEOF = charStream->read(c))) {
        backSlashNewLine = false;
        charStream->popMark();
        charStream->pushMark(); //want to retreat if it wasn't the case of backslash+newline
        if(*c == '\\') {
            char c2;
            bool notEOF2; //separate EOF boolean. If we encounter EOF in the next steps we simply backtrack and return the char
            if( (notEOF2 = charStream->read(&c2)) ) {
                if(c2 == '\r') {
                    charStream->popMark();
                    charStream->pushMark(); //i accept up to here, and update the mark accordingly
                    notEOF2 = charStream->read(&c2);
                    backSlashNewLine = true;
                }
                if(notEOF2 && c2 == '\n') {
                    //also takes into account CRLF, not only CR or LF, in a maximal munch strategy
                    charStream->popMark();
                    charStream->pushMark(); //i accept up to here, and update the mark accordingly
                    backSlashNewLine = true;
                }
            }
        }
    }
    charStream->resetToMark();
    charStream->popMark();
    return notEOF;
}

bool Lexer::readMaximumMunchWhile(std::string& wordToAppendTo, bool (*filter) (char)) {
    char c;
    bool eof_NOT_reached;
    charStream->pushMark();
    while( (eof_NOT_reached = readEliding(&c)) && filter(c)) {
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
    while( foundCount<terminator.size() && readEliding(&c) ) {
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
    bool notEOF=true, validEndOfFile=false;

    //Empty C source file is valid!
    if(emptyFile && !charStream->lookahead1(&c)) {
        return false;
    }
    emptyFile = false;

    charStream->pushMark();
    while( (notEOF = readEliding(&c)) && isspace(c))  {        
        //We don't want to come back to what we wasted
        charStream->popMark();
        charStream->pushMark();
    } //Wastes all whitespaces, newlines, etc.
    validEndOfFile = (c== '\n' || c=='\r'); //File must end in a newline!
    auto tp = positionOfLastChar(charStream); //Position for working tokens
    
    if(!notEOF) {
        if(!validEndOfFile) {
            tp = makeTokenPosition(charStream);
            token = std::make_shared<ErrorToken>(*tp, "Unexpected end of file");
        }
        charStream->popMark();
        return false;
        //Except in this case, even if we reached EOF, we want to say "true" so we can return the token for now. In the next iteration we'll terminate correctly or report source file not terminated by \n by this very block.
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
        bool leftovers = charStream->lookahead1(&c);
        if(token == nullptr || (leftovers && isCharOfIdentifier(c)) ) { //No keyword found OR keyword found but other letters are following
            charStream->resetToMark();
            readMaximumMunchWhile(word, isCharOfIdentifier);
            token = std::make_shared<IdentifierToken>(*tp, word);
        }
    }

    //Case: number constants
    else if (c == '0') {
        token = std::make_shared<ConstantToken>(
            *tp,
            ConstantType::Decimal,
            "0"
        );
    }

    else if(isdigit(c)) { //Nonzero decimal constant
        word.append(1,c);
        readMaximumMunchWhile(word, isDigit);
        token = std::make_shared<ConstantToken>(
            *tp,
            ConstantType::Decimal,
            word
        );
    }

    //Case: char constants
    else if(c=='\'') {
        notEOF = readEliding(&c);
        if (!notEOF) {
            tp = makeTokenPosition(charStream);
            token = std::make_shared<ErrorToken>(*tp, "EOF in the middle of a character constant");
            validToken = false;
        }

        //Inside the quotes
        if (validToken && c == '\\') { //Potential escape sequence!
            //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
            word.append(1, '\\');
            notEOF = readEliding(&c);
            validToken = notEOF && escapeSequence(c);
            if (!validToken) {
                if(notEOF)
                    tp = positionOfLastChar(charStream);
                else 
                    tp = makeTokenPosition(charStream);
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
        if (validToken && (notEOF = readEliding(&c)) && c=='\'') {
            token = std::make_shared<ConstantToken>(
                *tp,
                ConstantType::Character,
                word
            );
        }
        else if (/*it WAS a*/ validToken) { //we don't wanna overwrite error messages
            if(notEOF)
                tp = positionOfLastChar(charStream);
            else
                tp = makeTokenPosition(charStream);
            token = std::make_shared<ErrorToken>(*tp, "Expected ' to terminate the character constant");
            validToken = false;
        }
        //else i already have the errortoken ready
    }

    //Case: string
    else if (c=='\"') {
        DBGOUT("lexer", "Encountered string start");

        bool stringTerminated = false;
        while(!stringTerminated && readEliding(&c) && validToken) {
            if (c== '\"') { //String terminated correctly
                token = std::make_shared<ConstantToken>(
                    *tp,
                    ConstantType::String,
                    word
                );

                stringTerminated = true;
            }
            else if (c == '\\') { //Potential escape sequence!
                //We've read a '\'. If we reached EOF or there's an invalid escape sequence we reject.
                word.append(1, '\\');
                notEOF = readEliding(&c);
                bool validEscapeSequence = notEOF && escapeSequence(c);
                if (!validEscapeSequence) {
                    if(notEOF)
                        tp = positionOfLastChar(charStream);
                    else
                        tp = makeTokenPosition(charStream);
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
            token = std::make_shared<ConstantToken>(
                *tp,
                ConstantType::String,
                word
            );
        }
        else if(/*it WAS a*/ validToken) {
            validToken = false;
            tp = makeTokenPosition(charStream); //Eof reached, basically
            token = std::make_shared<ErrorToken>(*tp, "Unterminated string");
        }
    }

    else {
        bool commentFound = false;
        if(c == '/') {
        notEOF = readEliding(&c);
            if (notEOF) {
                if(c == '/') { //Single-line comment
                    // notEOF = readMaximumMunchUntil(word, "\n");
                    commentFound = true;
                    charStream->pushMark();
                    while( (notEOF = readEliding(&c)) && c!='\n' && c!='\r') {
                        charStream->popMark();
                        charStream->pushMark();
                    }
                    charStream->resetToMark();
                    charStream->popMark();
                }
                else if( c== '*') { //Multiline comment
                    commentFound = true;
                    notEOF = readMaximumMunchUntil(word, "*/");
                }

                if(commentFound) {
                    if(!notEOF) {
                        tp = makeTokenPosition(charStream);
                        token = std::make_shared<ErrorToken>(*tp, "Unterminated comment :(");
                        validToken = false;
                    }
                    else {
                        return nextToken(token); //We don't report any comment token. We keep going. This is tail recursion
                    }
                }
            }
        }
        if(!commentFound) { //last chance: Punctuator
            charStream->resetToMark();
            auto punctutator = this->punctuators->walk(*charStream)->getResult();

            if (punctutator != nullptr) {
                token = std::make_shared<PunctuatorToken>(*tp, *punctutator);
            } 
            else {
                charStream->read(&c); //Wastes the unrecognized char!
                tp = positionOfLastChar(charStream);
                token = std::make_shared<ErrorToken>(*tp, "Unrecognized symbol");
                validToken = false;
            }
        }
    }
        
    charStream->popMark();
    return CASE_ERRORTOKEN_RETURNS_TRUE || validToken; //read definition for clarification
}

}