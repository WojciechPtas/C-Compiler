#pragma once

#include <cctype>
#include <string>

#include "../../service/io/IBufferedInputStream.h"
#include "../../service/io/IInputStream.h"

namespace c4 {
    namespace util {
        namespace lexer {
            inline bool isNewlineNext(
                service::io::IInputStream<char> &src
            ) {
                char c;
                return src.read(&c) && (c == '\r' || c == '\n');
            }

            inline bool isNewlineNotNext(
                service::io::IInputStream<char> &src
            ) {
                char c;
                return src.read(&c) && c != '\r' && c != '\n';
            }

            inline bool isWhitespaceNext(
                service::io::IInputStream<char> &src
            ) {
                char c;
                return src.read(&c) && c != '\r' && c != '\n' && isspace(c);
            }

            template<typename TMatchFunction>
            inline bool skipAll(
                service::io::IBufferedInputStream<char> &src,
                TMatchFunction matchFn
            ) {
                auto atLeastOnce = false;
                src.pushMark();

                while (matchFn(src)) {
                    atLeastOnce = true;
                    src.moveMark();
                }

                src.resetAndPopMark();
                return atLeastOnce;
            }

            inline bool skipTwoCharacters(
                service::io::IBufferedInputStream<char> &src,
                char firstChar,
                char secondChar
            ) {
                char c;
                src.pushMark();

                if (
                    !src.read(&c) || c != firstChar || !src.read(&c) ||
                    c != secondChar
                ) {
                    src.resetAndPopMark();
                    return false;
                }

                src.popMark();
                return true;
            }
        }
    }
}
