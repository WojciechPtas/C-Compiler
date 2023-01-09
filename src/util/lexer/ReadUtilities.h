#pragma once

#include <cctype>
#include <string>

#include "../../service/io/IBufferedInputStream.h"
#include "../../service/io/IInputStream.h"

namespace c4 {
    namespace util {
        namespace lexer {
            inline bool isCharOfIdentifier(char c) {
                return c == '_' || std::isalnum(c);
            }

            inline bool isDigit(char c) {
                return std::isdigit(c);
            }

            inline bool isSimpleEscapeChar(char c) {
                return (c == '\'') || (c == '\"') || (c == '\?')
                    || (c == '\\') || (c == 'a' ) || (c == 'b' )
                    || (c == 'f' ) || (c == 'n' ) || (c == 'r' )
                    || (c == 't' ) || (c == 'v' );
            }

            inline bool isStartOfIdentifier(char c) {
                return c == '_' || std::isalpha(c);
            }

            template<typename TMatchFunction>
            inline void readAll(
                service::io::IBufferedInputStream<char> &src,
                TMatchFunction matchFn,
                std::string &dst
            ) {
                src.pushMark();

                for (char c; src.read(&c) && matchFn(c); src.moveMark()) {
                    dst.push_back(c);
                }

                src.resetAndPopMark();
            }
        }
    }
}
