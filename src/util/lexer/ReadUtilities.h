#pragma once

#include <cctype>
#include <string>
#include <stdexcept>
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

            inline char parseSingleChar(const std::string& str) {
                if(!str.empty()) {
                    if(str[0] == '\\' && str.length() > 1) {
                        switch(str[1]) {
                            case '\'':
                                return '\'';
                            case '\"':
                                return '\"';
                            case '\?':
                                return '\?';
                            case '\\':
                                return '\\';
                            case 'a':
                                return '\a';
                            case 'b':
                                return '\b';
                            case 'f':
                                return '\f';
                            case 'n':
                                return '\n';
                            case 'r':
                                return '\r';
                            case 't':
                                return '\t';
                            case 'v':
                                return '\v';
                            default:
                                throw std::logic_error("Malformed escape sequence");
                        }
                    }
                    else {
                        return str[0];
                    }
                }
                //We end here if we don't return anywhere else
                throw std::logic_error("Malformed character");
                return 0;
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
