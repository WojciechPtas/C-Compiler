#pragma once

#include <cstdint>
#include <string>

namespace c4 {
    namespace model {
        namespace token {
            class TokenPosition {
            public:
                const std::string file;
                const std::uint32_t line;
                const std::uint32_t column;

                TokenPosition(
                    std::string file,
                    std::uint32_t line,
                    std::uint32_t column
                ) : file(file), line(line), column(column) { }

                virtual ~TokenPosition() { }
            };
        }
    }
}
