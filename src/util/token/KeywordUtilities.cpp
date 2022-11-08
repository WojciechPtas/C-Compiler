#include <stdexcept>
#include "KeywordUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

const string &c4::util::token::stringify(Keyword keyword) {
    switch (keyword) {
        case Keyword::Auto:             return AUTO;
        case Keyword::Break:            return BREAK;
        case Keyword::Case:             return CASE;
        case Keyword::Char:             return CHAR;
        case Keyword::Const:            return CONST;
        case Keyword::Continue:         return CONTINUE;
        case Keyword::Default:          return DEFAULT;
        case Keyword::Do:               return DO;
        case Keyword::Double:           return DOUBLE;
        case Keyword::Else:             return ELSE;
        case Keyword::Enum:             return ENUM;
        case Keyword::Extern:           return EXTERN;
        case Keyword::Float:            return FLOAT;
        case Keyword::For:              return FOR;
        case Keyword::Goto:             return GOTO;
        case Keyword::If:               return IF;
        case Keyword::Inline:           return INLINE;
        case Keyword::Int:              return INT;
        case Keyword::Long:             return LONG;
        case Keyword::Register:         return REGISTER;
        case Keyword::Restrict:         return RESTRICT;
        case Keyword::Return:           return RETURN;
        case Keyword::Short:            return SHORT;
        case Keyword::Signed:           return SIGNED;
        case Keyword::Sizeof:           return SIZEOF;
        case Keyword::Static:           return STATIC;
        case Keyword::Struct:           return STRUCT;
        case Keyword::Switch:           return SWITCH;
        case Keyword::Typedef:          return TYPEDEF;
        case Keyword::Union:            return UNION;
        case Keyword::Unsigned:         return UNSIGNED;
        case Keyword::Void:             return VOID;
        case Keyword::Volatile:         return VOLATILE;
        case Keyword::While:            return WHILE;
        case Keyword::__Alignas:        return __ALIGNAS;
        case Keyword::__Alignof:        return __ALIGNOF;
        case Keyword::__Atomic:         return __ATOMIC;
        case Keyword::__Bool:           return __BOOL;
        case Keyword::__Complex:        return __COMPLEX;
        case Keyword::__Generic:        return __GENERIC;
        case Keyword::__Imaginary:      return __IMAGINARY;
        case Keyword::__Noreturn:       return __NORETURN;
        case Keyword::__Static_assert:  return __STATIC_ASSERT;
        case Keyword::__Thread_local:   return __THREAD_LOCAL;
        default:
            throw std::invalid_argument("punctuator");
    }
}
