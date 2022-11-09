#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            class ISO88591InputStream : public IInputStream<char> {
            public:
                ISO88591InputStream(std::string filePath);
                ~ISO88591InputStream();

                void popMark() override;
                void pushMark() override;
                bool read(char *dst) override;
                void resetToMark() override;

            private:
                std::ifstream stream;
                char *buffer;
                std::size_t bufferOffset;
                std::size_t bufferLimit;
                std::size_t bufferCapacity;
                std::vector<std::size_t> markStack;
            };
        }
    }
}
