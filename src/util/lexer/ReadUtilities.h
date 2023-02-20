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

            inline void parseEscapeSequences(std::string& str) {
                for(auto it=str.begin(); it<str.end(); it++) {
                    if(it+1 < str.end() && *it == '\\') {
                        switch(it[1]) {
                            case '\'':
                                str.replace(it, it+2, "\'");
                                break;
                            case '\"':
                                str.replace(it, it+2, "\"");
                                break;
                            case '\?':
                                str.replace(it, it+2, "\?");
                                break;
                            case '\\':
                                str.replace(it, it+2, "\\");
                                break;
                            case 'a':
                                str.replace(it, it+2, "\a");
                                break;
                            case 'b':
                                str.replace(it, it+2, "\b");
                                break;
                            case 'f':
                                str.replace(it, it+2, "\f");
                                break;
                            case 'n':
                                str.replace(it, it+2, "\n");
                                break;
                            case 'r':
                                str.replace(it, it+2, "\r");
                                break;
                            case 't':
                                str.replace(it, it+2, "\t");
                                break;
                            case 'v':
                                str.replace(it, it+2, "\v");
                                break;
                        }
                    }
                }
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
