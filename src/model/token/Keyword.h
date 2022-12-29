#pragma once

#include <type_traits>

namespace c4 {
    namespace model {
        namespace token {
            enum class Keyword : unsigned long long {
                Auto            = 0x0000000000000001L,
                Break           = 0x0000000000000002L,
                Case            = 0x0000000000000004L,
                Char            = 0x0000000000000008L,
                Const           = 0x0000000000000010L,
                Continue        = 0x0000000000000020L,
                Default         = 0x0000000000000040L,
                Do              = 0x0000000000000080L,
                Double          = 0x0000000000000100L,
                Else            = 0x0000000000000200L,
                Enum            = 0x0000000000000400L,
                Extern          = 0x0000000000000800L,
                Float           = 0x0000000000001000L,
                For             = 0x0000000000002000L,
                Goto            = 0x0000000000004000L,
                If              = 0x0000000000008000L,
                Inline          = 0x0000000000010000L,
                Int             = 0x0000000000020000L,
                Long            = 0x0000000000040000L,
                Register        = 0x0000000000080000L,
                Restrict        = 0x0000000000100000L,
                Return          = 0x0000000000200000L,
                Short           = 0x0000000000400000L,
                Signed          = 0x0000000000800000L,
                Sizeof          = 0x0000000001000000L,
                Static          = 0x0000000002000000L,
                Struct          = 0x0000000004000000L,
                Switch          = 0x0000000008000000L,
                Typedef         = 0x0000000010000000L,
                Union           = 0x0000000020000000L,
                Unsigned        = 0x0000000040000000L,
                Void            = 0x0000000080000000L,
                Volatile        = 0x0000000100000000L,
                While           = 0x0000000200000000L,
                __Alignas       = 0x0000000400000000L,
                __Alignof       = 0x0000000800000000L,
                __Atomic        = 0x0000001000000000L,
                __Bool          = 0x0000002000000000L,
                __Complex       = 0x0000004000000000L,
                __Generic       = 0x0000008000000000L,
                __Imaginary     = 0x0000010000000000L,
                __Noreturn      = 0x0000020000000000L,
                __Static_assert = 0x0000040000000000L,
                __Thread_local  = 0x0000080000000000L
            };

            inline Keyword operator& (Keyword l, Keyword r) {
                return Keyword(
                    std::underlying_type<Keyword>::type(l) &
                    std::underlying_type<Keyword>::type(r)
                );
            }

            inline Keyword operator| (Keyword l, Keyword r) {
                return Keyword(
                    std::underlying_type<Keyword>::type(l) |
                    std::underlying_type<Keyword>::type(r)
                );
            }
        }
    }
}
