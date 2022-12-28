#pragma once

#include <type_traits>

namespace c4 {
    namespace model {
        namespace token {
            enum class Punctuator : unsigned long long {
                LeftBracket                 = 0x0000000000000001L,
                RightBracket                = 0x0000000000000020L,
                LeftParenthesis             = 0x0000000000000040L,
                RightParenthesis            = 0x0000000000000080L,
                LeftBrace                   = 0x0000000000000100L,
                RightBrace                  = 0x0000000000000200L,
                Dot                         = 0x0000000000000400L,
                DashGreaterThan             = 0x0000000000000800L,
                DoublePlus                  = 0x0000000000001000L,
                DoubleMinus                 = 0x0000000000002000L,
                And                         = 0x0000000000004000L,
                Asterisk                    = 0x0000000000008000L,
                Plus                        = 0x0000000000010000L,
                Minus                       = 0x0000000000020000L,
                Tilde                       = 0x0000000000040000L,
                ExclamationMark             = 0x0000000000080000L,
                Slash                       = 0x0000000000100000L,
                Percent                     = 0x0000000000200000L,
                DoubleLessThan              = 0x0000000000400000L,
                DoubleGreaterThan           = 0x0000000000800000L,
                LessThan                    = 0x0000000001000000L,
                GreaterThan                 = 0x0000000002000000L,
                LessThanEqual               = 0x0000000004000000L,
                GreaterThanEqual            = 0x0000000008000000L,
                DoubleEqual                 = 0x0000000010000000L,
                ExclamationMarkEqual        = 0x0000000020000000L,
                Caret                       = 0x0000000040000000L,
                Pipe                        = 0x0000000080000000L,
                DoubleAnd                   = 0x0000000100000000L,
                DoublePipe                  = 0x0000000200000000L,
                QuestionMark                = 0x0000000400000000L,
                Colon                       = 0x0000000800000000L,
                Semicolon                   = 0x0000001000000000L,
                TripleDot                   = 0x0000002000000000L,
                Equal                       = 0x0000004000000000L,
                AsteriskEqual               = 0x0000008000000000L,
                SlashEqual                  = 0x0000010000000000L,
                PercentEqual                = 0x0000020000000000L,
                PlusEqual                   = 0x0000040000000000L,
                MinusEqual                  = 0x0000080000000000L,
                DoubleLessThanEqual         = 0x0000100000000000L,
                DoubleGreaterThanEqual      = 0x0000200000000000L,
                AndEqual                    = 0x0000400000000000L,
                CaretEqual                  = 0x0000800000000000L,
                PipeEqual                   = 0x0001000000000000L,
                Comma                       = 0x0002000000000000L,
                Hashtag                     = 0x0004000000000000L,
                DoubleHashtag               = 0x0008000000000000L,
                LessThanColon               = 0x0010000000000000L,
                ColonGreaterThan            = 0x0020000000000000L,
                LessThanPercent             = 0x0040000000000000L,
                PercentGreaterThan          = 0x0080000000000000L,
                PercentColon                = 0x0100000000000000L,
                PercentColonPercentColon    = 0x0200000000000000L
            };

            inline Punctuator operator| (Punctuator l, Punctuator r) {
                return Punctuator(
                    std::underlying_type<Punctuator>::type(l) |
                    std::underlying_type<Punctuator>::type(r)
                );
            }
        }
    }
}
