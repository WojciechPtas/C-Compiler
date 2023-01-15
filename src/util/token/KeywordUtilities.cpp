#include <stdexcept>

#include "../node/NodeUtilities.h"
#include "KeywordUtilities.h"

using namespace c4::model::node;
using namespace c4::model::token;
using namespace c4::util::node;
using namespace c4::util::token;
using namespace std;

static inline void _decomposeOne(
    set<Keyword> &dst,
    Keyword compoundKeyword,
    Keyword testedKeyword
);

const shared_ptr<const Node<char, Keyword>> c4::util::token::KEYWORD_TREE =
    buildTree(
        {
            makeBranch(AUTO, Keyword::Auto),
            makeBranch(BREAK, Keyword::Break),
            makeBranch(CASE, Keyword::Case),
            makeBranch(CHAR, Keyword::Char),
            makeBranch(CONST, Keyword::Const),
            makeBranch(CONTINUE, Keyword::Continue),
            makeBranch(DEFAULT, Keyword::Default),
            makeBranch(DO, Keyword::Do),
            makeBranch(DOUBLE, Keyword::Double),
            makeBranch(ELSE, Keyword::Else),
            makeBranch(ENUM, Keyword::Enum),
            makeBranch(EXTERN, Keyword::Extern),
            makeBranch(FLOAT, Keyword::Float),
            makeBranch(FOR, Keyword::For),
            makeBranch(GOTO, Keyword::Goto),
            makeBranch(IF, Keyword::If),
            makeBranch(INLINE, Keyword::Inline),
            makeBranch(INT, Keyword::Int),
            makeBranch(LONG, Keyword::Long),
            makeBranch(REGISTER, Keyword::Register),
            makeBranch(RESTRICT, Keyword::Restrict),
            makeBranch(RETURN, Keyword::Return),
            makeBranch(SHORT, Keyword::Short),
            makeBranch(SIGNED, Keyword::Signed),
            makeBranch(SIZEOF, Keyword::Sizeof),
            makeBranch(STATIC, Keyword::Static),
            makeBranch(STRUCT, Keyword::Struct),
            makeBranch(SWITCH, Keyword::Switch),
            makeBranch(TYPEDEF, Keyword::Typedef),
            makeBranch(UNION, Keyword::Union),
            makeBranch(UNSIGNED, Keyword::Unsigned),
            makeBranch(VOID, Keyword::Void),
            makeBranch(VOLATILE, Keyword::Volatile),
            makeBranch(WHILE, Keyword::While),
            makeBranch(__ALIGNAS, Keyword::__Alignas),
            makeBranch(__ALIGNOF, Keyword::__Alignof),
            makeBranch(__ATOMIC, Keyword::__Atomic),
            makeBranch(__BOOL, Keyword::__Bool),
            makeBranch(__COMPLEX, Keyword::__Complex),
            makeBranch(__GENERIC, Keyword::__Generic),
            makeBranch(__IMAGINARY, Keyword::__Imaginary),
            makeBranch(__NORETURN, Keyword::__Noreturn),
            makeBranch(__STATIC_ASSERT, Keyword::__Static_assert),
            makeBranch(__THREAD_LOCAL, Keyword::__Thread_local)
        }
    );

unique_ptr<set<Keyword>> c4::util::token::decompose(Keyword k) {
    auto r = make_unique<set<Keyword>>();

    _decomposeOne(*r, k, Keyword::Auto);
    _decomposeOne(*r, k, Keyword::Break);
    _decomposeOne(*r, k, Keyword::Case);
    _decomposeOne(*r, k, Keyword::Char);
    _decomposeOne(*r, k, Keyword::Const);
    _decomposeOne(*r, k, Keyword::Continue);
    _decomposeOne(*r, k, Keyword::Default);
    _decomposeOne(*r, k, Keyword::Do);
    _decomposeOne(*r, k, Keyword::Double);
    _decomposeOne(*r, k, Keyword::Else);
    _decomposeOne(*r, k, Keyword::Enum);
    _decomposeOne(*r, k, Keyword::Extern);
    _decomposeOne(*r, k, Keyword::Float);
    _decomposeOne(*r, k, Keyword::For);
    _decomposeOne(*r, k, Keyword::Goto);
    _decomposeOne(*r, k, Keyword::If);
    _decomposeOne(*r, k, Keyword::Inline);
    _decomposeOne(*r, k, Keyword::Int);
    _decomposeOne(*r, k, Keyword::Long);
    _decomposeOne(*r, k, Keyword::Register);
    _decomposeOne(*r, k, Keyword::Restrict);
    _decomposeOne(*r, k, Keyword::Return);
    _decomposeOne(*r, k, Keyword::Short);
    _decomposeOne(*r, k, Keyword::Signed);
    _decomposeOne(*r, k, Keyword::Sizeof);
    _decomposeOne(*r, k, Keyword::Static);
    _decomposeOne(*r, k, Keyword::Struct);
    _decomposeOne(*r, k, Keyword::Switch);
    _decomposeOne(*r, k, Keyword::Typedef);
    _decomposeOne(*r, k, Keyword::Union);
    _decomposeOne(*r, k, Keyword::Unsigned);
    _decomposeOne(*r, k, Keyword::Void);
    _decomposeOne(*r, k, Keyword::Volatile);
    _decomposeOne(*r, k, Keyword::While);
    _decomposeOne(*r, k, Keyword::__Alignas);
    _decomposeOne(*r, k, Keyword::__Alignof);
    _decomposeOne(*r, k, Keyword::__Atomic);
    _decomposeOne(*r, k, Keyword::__Bool);
    _decomposeOne(*r, k, Keyword::__Complex);
    _decomposeOne(*r, k, Keyword::__Generic);
    _decomposeOne(*r, k, Keyword::__Imaginary);
    _decomposeOne(*r, k, Keyword::__Noreturn);
    _decomposeOne(*r, k, Keyword::__Static_assert);
    _decomposeOne(*r, k, Keyword::__Thread_local);

    return r;
}

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
            throw std::invalid_argument("keyword");
    }
}

static inline void _decomposeOne(
    set<Keyword> &dst,
    Keyword compoundKeyword,
    Keyword testedKeyword
) {
    if ((compoundKeyword & testedKeyword) == testedKeyword) {
        dst.insert(testedKeyword);
    }
}
