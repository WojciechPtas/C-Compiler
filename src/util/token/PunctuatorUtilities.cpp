#include <stdexcept>

#include "../node/NodeUtilities.h"
#include "PunctuatorUtilities.h"

using namespace c4::model::node;
using namespace c4::model::token;
using namespace c4::util::node;
using namespace c4::util::token;
using namespace std;

static inline void _decomposeOne(
    set<Punctuator> &dst,
    Punctuator compoundPunctuator,
    Punctuator testedPunctuator
);

const shared_ptr<const Node<char, Punctuator>> c4::util::token::PUNCTUATOR_TREE =
    buildTree(
        {
            makeBranch(LEFT_BRACKET, Punctuator::LeftBracket),
            makeBranch(RIGHT_BRACKET, Punctuator::RightBracket),
            makeBranch(LEFT_PARENTHESIS, Punctuator::LeftParenthesis),
            makeBranch(RIGHT_PARENTHESIS, Punctuator::RightParenthesis),
            makeBranch(LEFT_BRACE, Punctuator::LeftBrace),
            makeBranch(RIGHT_BRACE, Punctuator::RightBrace),
            makeBranch(DOT, Punctuator::Dot),
            makeBranch(DASH_GREATER_THAN, Punctuator::DashGreaterThan),
            makeBranch(DOUBLE_PLUS, Punctuator::DoublePlus),
            makeBranch(DOUBLE_MINUS, Punctuator::DoubleMinus),
            makeBranch(AND, Punctuator::And),
            makeBranch(ASTERISK, Punctuator::Asterisk),
            makeBranch(PLUS, Punctuator::Plus),
            makeBranch(MINUS, Punctuator::Minus),
            makeBranch(TILDE, Punctuator::Tilde),
            makeBranch(EXCLAMATION_MARK, Punctuator::ExclamationMark),
            makeBranch(SLASH, Punctuator::Slash),
            makeBranch(PERCENT, Punctuator::Percent),
            makeBranch(DOUBLE_LESS_THAN, Punctuator::DoubleLessThan),
            makeBranch(DOUBLE_GREATER_THAN, Punctuator::DoubleGreaterThan),
            makeBranch(LESS_THAN, Punctuator::LessThan),
            makeBranch(GREATER_THAN, Punctuator::GreaterThan),
            makeBranch(LESS_THAN_EQUAL, Punctuator::LessThanEqual),
            makeBranch(GREATHER_THAN_EQUAL, Punctuator::GreaterThanEqual),
            makeBranch(DOUBLE_EQUAL, Punctuator::DoubleEqual),
            makeBranch(EXCLAMATION_MARK_EQUAL, Punctuator::ExclamationMarkEqual),
            makeBranch(CARET, Punctuator::Caret),
            makeBranch(PIPE, Punctuator::Pipe),
            makeBranch(DOUBLE_AND, Punctuator::DoubleAnd),
            makeBranch(DOUBLE_PIPE, Punctuator::DoublePipe),
            makeBranch(QUESTION_MARK, Punctuator::QuestionMark),
            makeBranch(COLON, Punctuator::Colon),
            makeBranch(SEMICOLON, Punctuator::Semicolon),
            makeBranch(TRIPLE_DOT, Punctuator::TripleDot),
            makeBranch(EQUAL, Punctuator::Equal),
            makeBranch(ASTERISK_EQUAL, Punctuator::AsteriskEqual),
            makeBranch(SLASH_EQUAL, Punctuator::SlashEqual),
            makeBranch(PERCENT_EQUAL, Punctuator::PercentEqual),
            makeBranch(PLUS_EQUAL, Punctuator::PlusEqual),
            makeBranch(MINUS_EQUAL, Punctuator::MinusEqual),
            makeBranch(DOUBLE_LESS_THAN_EQUAL, Punctuator::DoubleLessThanEqual),
            makeBranch(DOUBLE_GREATER_THAN_EQUAL, Punctuator::DoubleGreaterThanEqual),
            makeBranch(AND_EQUAL, Punctuator::AndEqual),
            makeBranch(CARET_EQUAL, Punctuator::CaretEqual),
            makeBranch(PIPE_EQUAL, Punctuator::PipeEqual),
            makeBranch(COMMA, Punctuator::Comma),
            makeBranch(HASHTAG, Punctuator::Hashtag),
            makeBranch(DOUBLE_HASHTAG, Punctuator::DoubleHashtag),
            makeBranch(LESS_THAN_COLON, Punctuator::LessThanColon),
            makeBranch(COLON_GREATER_THAN, Punctuator::ColonGreaterThan),
            makeBranch(LESS_THAN_PERCENT, Punctuator::LessThanPercent),
            makeBranch(PERCENT_GREATER_THAN, Punctuator::PercentGreaterThan),
            makeBranch(PERCENT_COLON, Punctuator::PercentColon),
            makeBranch(PERCENT_COLON_PERCENT_COLON, Punctuator::PercentColonPercentColon)
        }
    );

unique_ptr<set<Punctuator>> c4::util::token::decompose(Punctuator p) {
    auto r = make_unique<set<Punctuator>>();

    _decomposeOne(*r, p, Punctuator::LeftBracket);
    _decomposeOne(*r, p, Punctuator::RightBracket);
    _decomposeOne(*r, p, Punctuator::LeftParenthesis);
    _decomposeOne(*r, p, Punctuator::RightParenthesis);
    _decomposeOne(*r, p, Punctuator::LeftBrace);
    _decomposeOne(*r, p, Punctuator::RightBrace);
    _decomposeOne(*r, p, Punctuator::Dot);
    _decomposeOne(*r, p, Punctuator::DashGreaterThan);
    _decomposeOne(*r, p, Punctuator::DoublePlus);
    _decomposeOne(*r, p, Punctuator::DoubleMinus);
    _decomposeOne(*r, p, Punctuator::And);
    _decomposeOne(*r, p, Punctuator::Asterisk);
    _decomposeOne(*r, p, Punctuator::Plus);
    _decomposeOne(*r, p, Punctuator::Minus);
    _decomposeOne(*r, p, Punctuator::Tilde);
    _decomposeOne(*r, p, Punctuator::ExclamationMark);
    _decomposeOne(*r, p, Punctuator::Slash);
    _decomposeOne(*r, p, Punctuator::Percent);
    _decomposeOne(*r, p, Punctuator::DoubleLessThan);
    _decomposeOne(*r, p, Punctuator::DoubleGreaterThan);
    _decomposeOne(*r, p, Punctuator::LessThan);
    _decomposeOne(*r, p, Punctuator::GreaterThan);
    _decomposeOne(*r, p, Punctuator::LessThanEqual);
    _decomposeOne(*r, p, Punctuator::GreaterThanEqual);
    _decomposeOne(*r, p, Punctuator::DoubleEqual);
    _decomposeOne(*r, p, Punctuator::ExclamationMarkEqual);
    _decomposeOne(*r, p, Punctuator::Caret);
    _decomposeOne(*r, p, Punctuator::Pipe);
    _decomposeOne(*r, p, Punctuator::DoubleAnd);
    _decomposeOne(*r, p, Punctuator::DoublePipe);
    _decomposeOne(*r, p, Punctuator::QuestionMark);
    _decomposeOne(*r, p, Punctuator::Colon);
    _decomposeOne(*r, p, Punctuator::Semicolon);
    _decomposeOne(*r, p, Punctuator::TripleDot);
    _decomposeOne(*r, p, Punctuator::Equal);
    _decomposeOne(*r, p, Punctuator::AsteriskEqual);
    _decomposeOne(*r, p, Punctuator::SlashEqual);
    _decomposeOne(*r, p, Punctuator::PercentEqual);
    _decomposeOne(*r, p, Punctuator::PlusEqual);
    _decomposeOne(*r, p, Punctuator::MinusEqual);
    _decomposeOne(*r, p, Punctuator::DoubleLessThanEqual);
    _decomposeOne(*r, p, Punctuator::DoubleGreaterThanEqual);
    _decomposeOne(*r, p, Punctuator::AndEqual);
    _decomposeOne(*r, p, Punctuator::CaretEqual);
    _decomposeOne(*r, p, Punctuator::PipeEqual);
    _decomposeOne(*r, p, Punctuator::Comma);
    _decomposeOne(*r, p, Punctuator::Hashtag);
    _decomposeOne(*r, p, Punctuator::DoubleHashtag);
    _decomposeOne(*r, p, Punctuator::LessThanColon);
    _decomposeOne(*r, p, Punctuator::ColonGreaterThan);
    _decomposeOne(*r, p, Punctuator::LessThanPercent);
    _decomposeOne(*r, p, Punctuator::PercentGreaterThan);
    _decomposeOne(*r, p, Punctuator::PercentColon);
    _decomposeOne(*r, p, Punctuator::PercentColonPercentColon);

    return r;
}

const string &c4::util::token::stringify(Punctuator punctuator) {
    switch (punctuator) {
        case Punctuator::LeftBracket:
            return LEFT_BRACKET;
        case Punctuator::RightBracket:
            return RIGHT_BRACKET;
        case Punctuator::LeftParenthesis:
            return LEFT_PARENTHESIS;
        case Punctuator::RightParenthesis:
            return RIGHT_PARENTHESIS;
        case Punctuator::LeftBrace:
            return LEFT_BRACE;
        case Punctuator::RightBrace:
            return RIGHT_BRACE;
        case Punctuator::Dot:
            return DOT;
        case Punctuator::DashGreaterThan:
            return DASH_GREATER_THAN;
        case Punctuator::DoublePlus:
            return DOUBLE_PLUS;
        case Punctuator::DoubleMinus:
            return DOUBLE_MINUS;
        case Punctuator::And:
            return AND;
        case Punctuator::Asterisk:
            return ASTERISK;
        case Punctuator::Plus:
            return PLUS;
        case Punctuator::Minus:
            return MINUS;
        case Punctuator::Tilde:
            return TILDE;
        case Punctuator::ExclamationMark:
            return EXCLAMATION_MARK;
        case Punctuator::Slash:
            return SLASH;
        case Punctuator::Percent:
            return PERCENT;
        case Punctuator::DoubleLessThan:
            return DOUBLE_LESS_THAN;
        case Punctuator::DoubleGreaterThan:
            return DOUBLE_GREATER_THAN;
        case Punctuator::LessThan:
            return LESS_THAN;
        case Punctuator::GreaterThan:
            return GREATER_THAN;
        case Punctuator::LessThanEqual:
            return LESS_THAN_EQUAL;
        case Punctuator::GreaterThanEqual:
            return GREATHER_THAN_EQUAL;
        case Punctuator::DoubleEqual:
            return DOUBLE_EQUAL;
        case Punctuator::ExclamationMarkEqual:
            return EXCLAMATION_MARK_EQUAL;
        case Punctuator::Caret:
            return CARET;
        case Punctuator::Pipe:
            return PIPE;
        case Punctuator::DoubleAnd:
            return DOUBLE_AND;
        case Punctuator::DoublePipe:
            return DOUBLE_PIPE;
        case Punctuator::QuestionMark:
            return QUESTION_MARK;
        case Punctuator::Colon:
            return COLON;
        case Punctuator::Semicolon:
            return SEMICOLON;
        case Punctuator::TripleDot:
            return TRIPLE_DOT;
        case Punctuator::Equal:
            return EQUAL;
        case Punctuator::AsteriskEqual:
            return ASTERISK_EQUAL;
        case Punctuator::SlashEqual:
            return SLASH_EQUAL;
        case Punctuator::PercentEqual:
            return PERCENT_EQUAL;
        case Punctuator::PlusEqual:
            return PLUS_EQUAL;
        case Punctuator::MinusEqual:
            return MINUS_EQUAL;
        case Punctuator::DoubleLessThanEqual:
            return DOUBLE_LESS_THAN_EQUAL;
        case Punctuator::DoubleGreaterThanEqual:
            return DOUBLE_GREATER_THAN_EQUAL;
        case Punctuator::AndEqual:
            return AND_EQUAL;
        case Punctuator::CaretEqual:
            return CARET_EQUAL;
        case Punctuator::PipeEqual:
            return PIPE_EQUAL;
        case Punctuator::Comma:
            return COMMA;
        case Punctuator::Hashtag:
            return HASHTAG;
        case Punctuator::DoubleHashtag:
            return DOUBLE_HASHTAG;
        case Punctuator::LessThanColon:
            return LESS_THAN_COLON;
        case Punctuator::ColonGreaterThan:
            return COLON_GREATER_THAN;
        case Punctuator::LessThanPercent:
            return LESS_THAN_PERCENT;
        case Punctuator::PercentGreaterThan:
            return PERCENT_GREATER_THAN;
        case Punctuator::PercentColon:
            return PERCENT_COLON;
        case Punctuator::PercentColonPercentColon:
            return PERCENT_COLON_PERCENT_COLON;
        default:
            throw std::invalid_argument("punctuator");
    }
}

static inline void _decomposeOne(
    set<Punctuator> &dst,
    Punctuator compoundPunctuator,
    Punctuator testedPunctuator
) {
    if ((compoundPunctuator & testedPunctuator) == testedPunctuator) {
        dst.insert(testedPunctuator);
    }
}
