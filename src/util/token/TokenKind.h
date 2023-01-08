#pragma once
namespace c4::util::token{

union SpecifiedToken
    {
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