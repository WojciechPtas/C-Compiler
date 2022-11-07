#pragma once

#include <cstdint>
#include <string>

namespace c4 {
    namespace model {
        namespace token {
            class TokenPosition {
            public:
                TokenPosition(
                    std::string file,
                    std::uint32_t line,
                    std::uint32_t column
                );

                virtual ~TokenPosition() { }

                std::string getFile();
                std::uint32_t getLine();
                std::uint32_t getColumn();

            private:
                std::string file;
                std::uint32_t line;
                std::uint32_t column;
            };
        }
    }
}
