#pragma once

#include <cstdint>
#include <string>

#include "IBufferedInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class IFileInputStream : public IBufferedInputStream<TElement> {
            public:
                virtual ~IFileInputStream() { }

                virtual std::uint32_t getCurrentColumn() const = 0;
                virtual std::uint32_t getCurrentLine() const = 0;
                virtual std::uint32_t getLastReadColumn() const = 0;
                virtual std::uint32_t getLastReadLine() const = 0;
                virtual const std::string &getFilePath() const = 0;
                virtual bool lookahead(std::string& str, uint32_t amount) = 0;
            };
        }
    }
}
