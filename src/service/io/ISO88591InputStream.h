#pragma once

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <vector>

#include "IFileInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            struct ISO88591InputStreamMark {
                std::size_t bufferOffset;

                // Metrics snapshot

                std::uint32_t currentLine;
                std::uint32_t currentColumn;

                // Line recognition snapshot

                bool lastCharWasCR;
            };

            class ISO88591InputStream : public IFileInputStream<char> {
            public:
                ISO88591InputStream(std::string filePath);
                ~ISO88591InputStream();

                std::uint32_t getCurrentColumn() const;
                std::uint32_t getCurrentLine() const;
                const std::string &getFilePath() const;

                void popMark() override;
                void pushMark() override;
                bool read(char *dst) override;
                void resetToMark() override;

            private:
                const std::string filePath;

                std::ifstream stream;
                char *buffer;
                std::size_t bufferOffset;
                std::size_t bufferLimit;
                std::size_t bufferCapacity;
                std::vector<std::shared_ptr<const ISO88591InputStreamMark>> markStack;

                // Metric

                std::uint32_t currentLine;
                std::uint32_t currentColumn;

                // New-line recognition

                bool lastCharWasCR;

                void updateMetrics(char readCharacter);
            };
        }
    }
}
