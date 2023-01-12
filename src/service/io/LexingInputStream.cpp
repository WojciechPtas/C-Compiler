#include <cassert>

#include "../../model/token/ConstantToken.h"
#include "../../model/token/ErrorToken.h"
#include "../../model/token/IdentifierToken.h"
#include "../../model/token/KeywordToken.h"
#include "../../model/token/PunctuatorToken.h"

#include "../../util/lexer/PositionUtilities.h"
#include "../../util/lexer/ReadUtilities.h"
#include "../../util/lexer/SkipUtilities.h"

#include "LexingInputStream.h"

using namespace c4::model;
using namespace c4::model::token;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace c4::util::lexer;
using namespace std;

LexingInputStream::LexingInputStream(
    const shared_ptr<MetricInputStream> &src,
    const shared_ptr<const IAutomaton<char, Keyword>> &keywordAutomaton,
    const shared_ptr<const IAutomaton<char, Punctuator>> &punctuatorAutomaton
) : keywordAutomaton(keywordAutomaton),
    punctuatorAutomaton(punctuatorAutomaton),
    source(src),
    eofReached(false),
    isEmpty(true) { }

bool LexingInputStream::read(shared_ptr<Token> *dst) {
    char nextChar;

    // If we hit EOF right at the beginning, we encountered an empty stream,
    // which is valid, but lacks any tokens.

    this->eofReached |= this->isEmpty && !this->source->peek(&nextChar);

    if (this->eofReached) {
        return false;
    }

    this->isEmpty = false;

    // NOTE:    Since the project specification states that we may omit
    //          §5.1.1.2, point 2, combining lines that end with a backslash,
    //          we do not consider them here.
    //
    // Skip everything that is considered as whitespace or comment.

    bool continueSkipping, lastSkippedNewline;

    do {
        continueSkipping = skipAll(*this->source, isWhitespaceNext);

        if (skipTwoCharacters(*this->source, '/', '/')) {
            skipAll(*this->source, isNewlineNotNext);
            continueSkipping = true;
        }

        if (skipTwoCharacters(*this->source, '/', '*')) {
            for (
                char lastChar = 0;

                this->source->read(&nextChar) &&
                (lastChar != '*' || nextChar != '/');

                lastChar = nextChar
            ) ;

            continueSkipping = true;
        }

        lastSkippedNewline &= !continueSkipping;

        auto skippedNewline = skipAll(*this->source, isNewlineNext);
        lastSkippedNewline |= skippedNewline;
        continueSkipping |= skippedNewline;
    } while (continueSkipping);

    // If EOF reached, make sure that we have a valid file ending, i.e. file
    // terminates with a newline.

    if (!this->source->peek(&nextChar)) {
        this->eofReached = true;

        if (lastSkippedNewline) {
            return false;
        }

        MAKE_TOKEN_POSITION(tokenPosition, *this->source);

        *dst = make_unique<ErrorToken>(
            tokenPosition,
            "Unexpected end of file"
        );

        return true;
    }
    
    // Case: Keyword or Identifier

    if (isStartOfIdentifier(nextChar)) {
        this->readIdentifierOrKeyword(dst);
        return true;
    }

    // Case: Decimal Constant

    if (isDigit(nextChar)) {
        this->readDecimalConstant(dst);
        return true;
    }

    // Case: Character Constant

    if (nextChar == '\'') {
        this->readCharacterConstant(dst);
        return true;
    }

    // Case: String Constant

    if (nextChar == '"') {
        this->readStringLiteral(dst);
        return true;
    }

    // Case: Punctuator (or error)

    this->readPunctuatorOrError(dst);
    return true;
}

void LexingInputStream::readCharacterConstant(shared_ptr<Token> *dst) {
    MAKE_TOKEN_POSITION(constantBeginning, *this->source);
    string constant;
    char nextChar;

    // Invariant:   The next character in the underlying stream is a single
    //              quote.

    assert(this->source->read(&nextChar) && nextChar == '\'');

    // Reading the actual content
    
    MAKE_TOKEN_POSITION(contentBeginning, *this->source);

    if ((this->eofReached = !this->source->read(&nextChar))) {
        *dst = make_unique<ErrorToken>(
            contentBeginning,
            "EOF in the middle of a character constant"
        );

        return;
    }

    // If the character is a single-quote or a new-line, we have an error.

    if (nextChar == '\'' || nextChar == '\r' || nextChar == '\n') {
        this->eofReached = true;

        *dst = make_unique<ErrorToken>(
            contentBeginning,
            "Disallowed character in character constant"
        );

        return;
    }

    constant.push_back(nextChar);

    // If the character is a backslash, we have to check for valid escape
    // sequences.

    if (nextChar == '\\') {
        MAKE_TOKEN_POSITION(escapeCharacter, *this->source);

        if (!this->source->read(&nextChar) || !isSimpleEscapeChar(nextChar)) {
            this->eofReached = true;

            *dst = make_unique<ErrorToken>(
                escapeCharacter,
                "Invalid escape sequence"
            );

            return;
        }

        constant.push_back(nextChar);
    }

    // Expecting the character constant terminating single quotation mark
    // delimiter.

    MAKE_TOKEN_POSITION(constantEnding, *this->source);

    if (!this->source->read(&nextChar) || nextChar != '\'') {
        this->eofReached = true;

        *dst = make_unique<ErrorToken>(
            constantEnding,
            "Expected ' to terminate the character constant"
        );

        return;
    }

    // Everything fine - we have a valid character constant.

    *dst = make_unique<ConstantToken>(
        constantBeginning,
        ConstantType::Character,
        constant
    );
}

void LexingInputStream::readDecimalConstant(shared_ptr<Token> *dst) {
    MAKE_TOKEN_POSITION(tokenPosition, *this->source);
    string constant;
    char firstChar;

    // Invariant:   Since this method has been called because the next
    //              character in the stream is a digit, we add this to the
    //              constant buffer immediately.

    assert(this->source->read(&firstChar));
    constant.push_back(firstChar);

    // If the firstChar is anything but a zero, we continue reading the digits
    // until there are no more left. (In case of a zero, we build a single zero
    // digit constant.)

    if (firstChar != '0') {
        readAll(*this->source, isDigit, constant);
    }

    *dst = make_unique<ConstantToken>(
        tokenPosition,
        ConstantType::Decimal,
        constant
    );
}

void LexingInputStream::readIdentifierOrKeyword(shared_ptr<Token> *dst) {
    MAKE_TOKEN_POSITION(tokenPosition, *this->source);
    char nextChar;

    // First attempt: Read a keyword.

    this->source->pushMark();
    auto keywordState = this->keywordAutomaton->walk(*this->source);
    this->eofReached = !this->source->peek(&nextChar);

    if (
        keywordState != nullptr &&
        (this->eofReached || !isCharOfIdentifier(nextChar))
    ) {
        *dst = make_unique<KeywordToken>(
            tokenPosition,
            *keywordState->getResult()
        );

        this->source->popMark();
        return;
    }

    this->source->resetAndPopMark();

    // Second attempt: Read everything that matches an identifier.
    //
    // Invariant:   Since this method has been called because the next
    //              character in the stream is a valid identifier beginning, we
    //              can add this to the identifier buffer without any further
    //              checks.

    string identifier;

    assert(this->source->read(&nextChar));
    identifier.push_back(nextChar);
    readAll(*this->source, isCharOfIdentifier, identifier);

    *dst = make_unique<IdentifierToken>(tokenPosition, identifier);
}

void LexingInputStream::readPunctuatorOrError(shared_ptr<Token> *dst) {
    MAKE_TOKEN_POSITION(tokenPosition, *this->source);

    auto punctuatorNode = this->punctuatorAutomaton->walk(*this->source);

    if (punctuatorNode == nullptr) {
        *dst = make_unique<ErrorToken>(tokenPosition, "Unrecognized symbol");
        this->eofReached = true;
        return;
    }

    *dst = make_unique<PunctuatorToken>(
        tokenPosition,
        *punctuatorNode->getResult()
    );
}

void LexingInputStream::readStringLiteral(shared_ptr<Token> *dst) {
    MAKE_TOKEN_POSITION(tokenPosition, *this->source);
    bool inEscapeSequence = false;
    string literal;
    char nextChar;

    // Invariant:   The next character is a double quotation mark that we can
    //              skip safely.

    assert(this->source->read(&nextChar) && nextChar == '"');

    // Reading until we either encounter an invalid escape sequence, a
    // (forbidden) newline or the terminating double quotation mark.
    //
    // NOTE:    An appropriate loop condition would be unreadable, hence we
    //          rather use return/break/continue statements for control flow.

    while (true) {
        MAKE_TOKEN_POSITION(erroneousPosition, *this->source);

        if ((this->eofReached = !this->source->read(&nextChar))) {
            
            *dst = make_unique<ErrorToken>(
                erroneousPosition,
                "Unterminated string"
            );

            return;
        }

        if (nextChar == '\r' || nextChar == '\n') {
            *dst = make_unique<ErrorToken>(
                erroneousPosition,
                "Newlines in string literals are not allowed"
            );

            this->eofReached = true;
            return;
        }

        if (!inEscapeSequence) {
            if (nextChar == '"') {
                break;
            }

            if (nextChar == '\\') {
                inEscapeSequence = true;
            }
        } else if (!isSimpleEscapeChar(nextChar)) {
            *dst = make_unique<ErrorToken>(
                erroneousPosition,
                "Invalid escape sequence"
            );

            this->eofReached = true;
            return;
        } else {
            inEscapeSequence = false;
        }
        
        literal.push_back(nextChar);
    }

    *dst = make_unique<ConstantToken>(
        tokenPosition,
        ConstantType::String,
        literal
    );
}
