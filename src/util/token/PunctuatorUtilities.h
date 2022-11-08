#pragma once

#include <string>
#include "../../model/token/Punctuator.h"

namespace c4 {
    namespace util {
        namespace token {
            const std::string LEFT_BRACKET                  = "[";
            const std::string RIGHT_BRACKET                 = "]";
            const std::string LEFT_PARENTHESIS              = "(";
            const std::string RIGHT_PARENTHESIS             = ")";
            const std::string LEFT_BRACE                    = "{";
            const std::string RIGHT_BRACE                   = "}";
            const std::string DOT                           = ".";
            const std::string DASH_GREATER_THAN             = "->";
            const std::string DOUBLE_PLUS                   = "++";
            const std::string DOUBLE_MINUS                  = "--";
            const std::string AND                           = "&";
            const std::string ASTERISK                      = "*";
            const std::string PLUS                          = "+";
            const std::string MINUS                         = "-";
            const std::string TILDE                         = "~";
            const std::string EXCLAMATION_MARK              = "!";
            const std::string SLASH                         = "/";
            const std::string PERCENT                       = "%";
            const std::string DOUBLE_LESS_THAN              = "<<";
            const std::string DOUBLE_GREATER_THAN           = ">>";
            const std::string LESS_THAN                     = "<";
            const std::string GREATER_THAN                  = ">";
            const std::string LESS_THAN_EQUAL               = "<=";
            const std::string GREATHER_THAN_EQUAL           = ">=";
            const std::string DOUBLE_EQUAL                  = "==";
            const std::string EXCLAMATION_MARK_EQUAL        = "!=";
            const std::string CARET                         = "^";
            const std::string PIPE                          = "|";
            const std::string DOUBLE_AND                    = "&&";
            const std::string DOUBLE_PIPE                   = "||";
            const std::string QUESTION_MARK                 = "?";
            const std::string COLON                         = ":";
            const std::string SEMICOLON                     = ";";
            const std::string TRIPLE_DOT                    = "...";
            const std::string EQUAL                         = "=";
            const std::string ASTERISK_EQUAL                = "*=";
            const std::string SLASH_EQUAL                   = "/=";
            const std::string PERCENT_EQUAL                 = "%=";
            const std::string PLUS_EQUAL                    = "+=";
            const std::string MINUS_EQUAL                   = "-=";
            const std::string DOUBLE_LESS_THAN_EQUAL        = "<<=";
            const std::string DOUBLE_GREATER_THAN_EQUAL     = ">>=";
            const std::string AND_EQUAL                     = "&=";
            const std::string CARET_EQUAL                   = "^=";
            const std::string PIPE_EQUAL                    = "|=";
            const std::string COMMA                         = ",";
            const std::string HASHTAG                       = "#";
            const std::string DOUBLE_HASHTAG                = "##";
            const std::string LESS_THAN_COLON               = "<:";
            const std::string COLON_GREATER_THAN            = ":>";
            const std::string LESS_THAN_PERCENT             = "<%";
            const std::string PERCENT_GREATER_THAN          = "%>";
            const std::string PERCENT_COLON                 = "%:";
            const std::string PERCENT_COLON_PERCENT_COLON   = "%:%:";

            const std::string &stringify(model::token::Punctuator punctuator);
        }
    }
}
