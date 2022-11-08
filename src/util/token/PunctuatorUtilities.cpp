#include "PunctuatorUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;

const std::string &stringify(Punctuator punctuator) {
    switch (punctuator) {
        case Punctuator::LeftBracket:               return "[";
        case Punctuator::RightBracket:              return "]";
        case Punctuator::LeftParenthesis:           return "(";
        case Punctuator::RightParenthesis:          return ")";
        case Punctuator::LeftBrace:                 return "{";
        case Punctuator::RightBrace:                return "}";
        case Punctuator::Dot:                       return ".";
        case Punctuator::DashGreaterThan:           return "->";
        case Punctuator::DoublePlus:                return "++";
        case Punctuator::DoubleMinus:               return "--";
        case Punctuator::And:                       return "&";
        case Punctuator::Asterisk:                  return "*";
        case Punctuator::Plus:                      return "+";
        case Punctuator::Minus:                     return "-";
        case Punctuator::Tilde:                     return "~";
        case Punctuator::ExclamationMark:           return "!";
        case Punctuator::Slash:                     return "/";
        case Punctuator::Percent:                   return "%";
        case Punctuator::DoubleLessThan:            return "<<";
        case Punctuator::DoubleGreaterThan:         return ">>";
        case Punctuator::LessThan:                  return "<";
        case Punctuator::GreaterThan:               return ">";
        case Punctuator::LessThanEqual:             return "<=";
        case Punctuator::GreaterThanEqual:          return ">=";
        case Punctuator::DoubleEqual:               return "==";
        case Punctuator::ExclamationMarkEqual:      return "!=";
        case Punctuator::Caret:                     return "^";
        case Punctuator::Pipe:                      return "|";
        case Punctuator::DoubleAnd:                 return "&&";
        case Punctuator::DoublePipe:                return "||";
        case Punctuator::QuestionMark:              return "?";
        case Punctuator::Colon:                     return ":";
        case Punctuator::Semicolon:                 return ";";
        case Punctuator::TripleDot:                 return "...";
        case Punctuator::Equal:                     return "=";
        case Punctuator::AsteriskEqual:             return "*=";
        case Punctuator::SlashEqual:                return "/=";
        case Punctuator::PercentEqual:              return "%=";
        case Punctuator::PlusEqual:                 return "+=";
        case Punctuator::MinusEqual:                return "-=";
        case Punctuator::DoubleLessThanEqual:       return "<<=";
        case Punctuator::DoubleGreaterThanEqual:    return ">>=";
        case Punctuator::AndEqual:                  return "&=";
        case Punctuator::CaretEqual:                return "^=";
        case Punctuator::PipeEqual:                 return "|=";
        case Punctuator::Comma:                     return ",";
        case Punctuator::Hashtag:                   return "#";
        case Punctuator::DoubleHashtag:             return "##";
        case Punctuator::LessThanColon:             return "<:";
        case Punctuator::ColonGreaterThan:          return ":>";
        case Punctuator::LessThanPercent:           return "<%";
        case Punctuator::PercentGreaterThan:        return "%>";
        case Punctuator::PercentColon:              return "%:";
        case Punctuator::PercentColonPercentColon:  return "%:%:";
    }
}
