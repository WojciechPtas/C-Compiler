#pragma once

#include "../../../model/parser/lr/LookaheadCondition.h"
#include "../../../model/token/Keyword.h"
#include "../../../model/token/Punctuator.h"

namespace c4 {
    namespace util {
        namespace parser {
            namespace lr {
                const model::parser::lr::LookaheadCondition ANY_TOKEN = {
                    model::parser::lr::TokenType::Constant |
                    model::parser::lr::TokenType::End |
                    model::parser::lr::TokenType::Error |
                    model::parser::lr::TokenType::Identifier |
                    model::parser::lr::TokenType::Keyword |
                    model::parser::lr::TokenType::Punctuator,
                    model::token::Keyword(0x00000FFFFFFFFFFFL),
                    model::token::Punctuator(0x03FFFFFFFFFFFFFFL)
                };

                const model::parser::lr::LookaheadCondition CONSTANT_TOKEN = {
                    model::parser::lr::TokenType::Constant,
                    model::token::Keyword(0),
                    model::token::Punctuator(0)
                };

                const model::parser::lr::LookaheadCondition END_TOKEN = {
                    model::parser::lr::TokenType::End,
                    model::token::Keyword(0),
                    model::token::Punctuator(0)
                };

                const model::parser::lr::LookaheadCondition ERROR_TOKEN = {
                    model::parser::lr::TokenType::Error,
                    model::token::Keyword(0),
                    model::token::Punctuator(0)
                };

                const model::parser::lr::LookaheadCondition IDENTIFIER_TOKEN = {
                    model::parser::lr::TokenType::Identifier,
                    model::token::Keyword(0),
                    model::token::Punctuator(0)
                };

                const model::parser::lr::LookaheadCondition UNARY_EXPRESSION_TOKENS = {
                    model::parser::lr::TokenType::Punctuator | 
                        model::parser::lr::TokenType::Constant |
                        model::parser::lr::TokenType::Identifier |
                        model::parser::lr::TokenType::Keyword,

                    model::token::Keyword::Sizeof,
                        
                    model::token::Punctuator::And |
                        model::token::Punctuator::Asterisk |
                        model::token::Punctuator::ExclamationMark |
                        model::token::Punctuator::Minus |
                        model::token::Punctuator::LeftParenthesis
                };

                inline model::parser::lr::LookaheadCondition KEYWORD_TOKEN(
                    model::token::Keyword keyword
                ) {
                    model::parser::lr::LookaheadCondition condition;
                    condition.typeMask = model::parser::lr::TokenType::Keyword;
                    condition.keywordMask = keyword;

                    return condition;
                }

                inline model::parser::lr::LookaheadCondition PUNCTUATOR_TOKEN(
                    model::token::Punctuator punctuator
                ) {
                    model::parser::lr::LookaheadCondition condition;
                    condition.typeMask =
                        model::parser::lr::TokenType::Punctuator;

                    condition.punctuatorMask = punctuator;

                    return condition;
                }
            }
        }
    }
}
