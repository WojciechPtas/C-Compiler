#pragma once

#include <cstddef>
#include <string_view>

namespace c4 {
    namespace model {
        namespace token {
            class TokenPosition {
            public:
                const std::string_view file;
                const std::size_t line;
                const std::size_t column;

                TokenPosition(const TokenPosition&) = default;

                TokenPosition(
                    const std::string_view &file,
                    std::size_t line,
                    std::size_t column
                ) : file(file), line(line), column(column) { }

                virtual ~TokenPosition() { }

            };
            static const TokenPosition TOKENPOSITION_UNSET = TokenPosition("Position not set", 0, 0);
        }
    }
}
