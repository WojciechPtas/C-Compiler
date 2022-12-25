#pragma once

#include <cstdint>

namespace c4 {
    namespace model {
        namespace parser {
            class Reduction {
            public:
                const std::uint32_t stateConsumption;
                const std::uint32_t expressionConsumption;

                virtual ~Reduction() { }

            protected:
                Reduction(
                    std::uint32_t stateConsumption,
                    std::uint32_t expressionConsumption
                );
            };
        }
    }
}
