#pragma once

namespace c4 {
    namespace model {
        namespace token {
            enum class Punctuator {
                LeftBracket, RightBracket, LeftParenthesis, RightParenthesis,
                LeftBrace, RightBrace, Dot, DashGreaterThan, DoublePlus,
                DoubleMinus, And, Asterisk, Plus, Minus, Tilde,
                ExclamationMark, Slash, Percent, DoubleLessThan,
                DoubleGreaterThan, LessThan, GreaterThan, LessThanEqual,
                GreaterThanEqual, DoubleEqual, ExclamationMarkEqual, Caret,
                Pipe, DoubleAnd, DoublePipe, QuestionMark, Colon, Semicolon,
                TripleDot, Equal, AsteriskEqual, SlashEqual, PercentEqual,
                PlusEqual, MinusEqual, DoubleLessThanEqual,
                DoubleGreaterThanEqual, AndEqual, CaretEqual, PipeEqual, Comma,
                Hashtag, DoubleHashtag, LessThanColon, ColonGreaterThan,
                LessThanPercent, PercentGreaterThan, PercentColon,
                PercentColonPercentColon
            };
        }
    }
}
