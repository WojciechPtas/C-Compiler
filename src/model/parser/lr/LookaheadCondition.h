#pragma once

#include <type_traits>

#include "../../token/Keyword.h"
#include "../../token/Punctuator.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                enum class TokenType : unsigned char {
                    Constant    = 0x01,
                    End         = 0x02,
                    Error       = 0x04,
                    Identifier  = 0x08,
                    Keyword     = 0x10,
                    Punctuator  = 0x20
                };

                struct LookaheadCondition {
                    TokenType typeMask;
                    token::Keyword keywordMask;
                    token::Punctuator punctuatorMask;
                };

                inline TokenType operator| (TokenType l, TokenType r) {
                    return TokenType(
                        std::underlying_type<TokenType>::type(l) |
                        std::underlying_type<TokenType>::type(r)
                    );
                }

                inline LookaheadCondition operator| (
                    LookaheadCondition l,
                    LookaheadCondition r
                ) {
                    return {
                        l.typeMask | r.typeMask,
                        l.keywordMask | r.keywordMask,
                        l.punctuatorMask | r.punctuatorMask
                    };
                }
            }
        }
    }
}
