#pragma once

#include <fstream>
#include <string>

#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            class FileInputStream : public IInputStream<char> {
            public:
                FileInputStream(const std::string &path) : source(path) { }

                bool read(char *dst) override {
                    return this->source.read(dst, 1).gcount() == 1;
                }

                std::size_t read(char *dst, std::size_t maxCount) override {
                    return this->source.read(dst, maxCount).gcount();
                }

            private:
                std::ifstream source;
            };
        }
    }
}
