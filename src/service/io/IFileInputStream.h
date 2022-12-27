#pragma once

#include <cstdint>
#include <string>

#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class IFileInputStream : public IInputStream<TElement> {
            public:
                virtual ~IFileInputStream() { }

                virtual std::uint32_t getCurrentColumn() const = 0;
                virtual std::uint32_t getCurrentLine() const = 0;
                virtual std::uint32_t getLastReadColumn() const = 0;
                virtual std::uint32_t getLastReadLine() const = 0;
                virtual const std::string &getFilePath() const = 0;
            };
        }
    }
}
