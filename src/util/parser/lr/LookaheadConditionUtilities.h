#pragma once

#include "../../../model/parser/lr/LookaheadCondition.h"

namespace c4 {
    namespace util {
        namespace parser {
            namespace lr {
                const c4::model::parser::lr::LookaheadCondition CONSTANT_TOKEN = {
                    c4::model::parser::lr::TokenType::Constant
                };

                const c4::model::parser::lr::LookaheadCondition END = {
                    c4::model::parser::lr::TokenType::End
                };

                const c4::model::parser::lr::LookaheadCondition ERROR_TOKEN = {
                    c4::model::parser::lr::TokenType::Error
                };

                const c4::model::parser::lr::LookaheadCondition IDENTIFIER_TOKEN = {
                    c4::model::parser::lr::TokenType::Identifier
                };

                inline c4::model::parser::lr::LookaheadCondition KEYWORD_TOKEN(
                    c4::model::token::Keyword keyword
                ) {
                    c4::model::parser::lr::LookaheadCondition condition;
                    condition.typeMask =
                        c4::model::parser::lr::TokenType::Keyword;

                    condition.keywordMask = keyword;

                    return condition;
                }

                inline c4::model::parser::lr::LookaheadCondition PUNCTUATOR_TOKEN(
                    c4::model::token::Punctuator punctuator
                ) {
                    c4::model::parser::lr::LookaheadCondition condition;
                    condition.typeMask =
                        c4::model::parser::lr::TokenType::Punctuator;

                    condition.punctuatorMask = punctuator;

                    return condition;
                }
            }
        }
    }
}
