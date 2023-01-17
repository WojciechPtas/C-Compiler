#pragma once
namespace c4::util::token{

class SpecifiedToken
    {
        public:
        SpecifiedToken(): empty(true){};
        SpecifiedToken(model::token::Punctuator p) : p(p) {};
        SpecifiedToken(model::token::Keyword k) : k(k) {};
        bool empty;
        model::token::Punctuator p;
        model::token::Keyword k;
    };
enum class TokenKind{
    identifier,
    punctuator,
    character_constant,
    constant,
    error,
    string_literal,
    keyword
    };


    }