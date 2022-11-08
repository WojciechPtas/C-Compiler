#include "KeywordUtilities.h"

using namespace c4::model::token;
using namespace c4::util::token;

// TODO: Out-source returned strings to constants to avoid errors.

const std::string &stringify(Keyword keyword) {
    switch (keyword) {
        case Keyword::Auto:             return "auto";
        case Keyword::Break:            return "break";
        case Keyword::Case:             return "case";
        case Keyword::Char:             return "char";
        case Keyword::Const:            return "const";
        case Keyword::Continue:         return "continue";
        case Keyword::Default:          return "default";
        case Keyword::Do:               return "do";
        case Keyword::Double:           return "double";
        case Keyword::Else:             return "else";
        case Keyword::Enum:             return "enum";
        case Keyword::Extern:           return "extern";
        case Keyword::Float:            return "float";
        case Keyword::For:              return "for";
        case Keyword::Goto:             return "goto";
        case Keyword::If:               return "if";
        case Keyword::Inline:           return "inline";
        case Keyword::Int:              return "int";
        case Keyword::Long:             return "long";
        case Keyword::Register:         return "register";
        case Keyword::Restrict:         return "restrict";
        case Keyword::Return:           return "return";
        case Keyword::Short:            return "short";
        case Keyword::Signed:           return "signed";
        case Keyword::Sizeof:           return "sizeof";
        case Keyword::Static:           return "static";
        case Keyword::Struct:           return "struct";
        case Keyword::Switch:           return "switch";
        case Keyword::Typedef:          return "typedef";
        case Keyword::Union:            return "union";
        case Keyword::Unsigned:         return "unsigned";
        case Keyword::Void:             return "void";
        case Keyword::Volatile:         return "volatile";
        case Keyword::While:            return "while";
        case Keyword::__Alignas:        return "_Alignas";
        case Keyword::__Alignof:        return "_Alignof";
        case Keyword::__Atomic:         return "_Atomic";
        case Keyword::__Bool:           return "_Bool";
        case Keyword::__Complex:        return "_Complex";
        case Keyword::__Generic:        return "_Generic";
        case Keyword::__Imaginary:      return "_Imaginary";
        case Keyword::__Noreturn:       return "_Noreturn";
        case Keyword::__Static_assert:  return "_Static_assert";
        case Keyword::__Thread_local:   return "_Thread_local";
    }
}
