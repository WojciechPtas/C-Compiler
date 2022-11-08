#pragma once

#include <string>
#include "../../model/token/Keyword.h"

namespace c4 {
    namespace util {
        namespace token {
            const std::string AUTO              = "auto";
            const std::string BREAK             = "break";
            const std::string CASE              = "case";
            const std::string CHAR              = "char";
            const std::string CONST             = "const";
            const std::string CONTINUE          = "continue";
            const std::string DEFAULT           = "default";
            const std::string DO                = "do";
            const std::string DOUBLE            = "double";
            const std::string ELSE              = "else";
            const std::string ENUM              = "enum";
            const std::string EXTERN            = "extern";
            const std::string FLOAT             = "float";
            const std::string FOR               = "for";
            const std::string GOTO              = "goto";
            const std::string IF                = "if";
            const std::string INLINE            = "inline";
            const std::string INT               = "int";
            const std::string LONG              = "long";
            const std::string REGISTER          = "register";
            const std::string RESTRICT          = "restrict";
            const std::string RETURN            = "return";
            const std::string SHORT             = "short";
            const std::string SIGNED            = "signed";
            const std::string SIZEOF            = "sizeof";
            const std::string STATIC            = "static";
            const std::string STRUCT            = "struct";
            const std::string SWITCH            = "switch";
            const std::string TYPEDEF           = "typedef";
            const std::string UNION             = "union";
            const std::string UNSIGNED          = "unsigned";
            const std::string VOID              = "void";
            const std::string VOLATILE          = "volatile";
            const std::string WHILE             = "while";
            const std::string __ALIGNAS         = "_Alignas";
            const std::string __ALIGNOF         = "_Alignof";
            const std::string __ATOMIC          = "_Atomic";
            const std::string __BOOL            = "_Bool";
            const std::string __COMPLEX         = "_Complex";
            const std::string __GENERIC         = "_Generic";
            const std::string __IMAGINARY       = "_Imaginary";
            const std::string __NORETURN        = "_Noreturn";
            const std::string __STATIC_ASSERT   = "_Static_assert";
            const std::string __THREAD_LOCAL    = "_Thread_local";

            const std::string &stringify(model::token::Keyword keyword);
        }
    }
}
