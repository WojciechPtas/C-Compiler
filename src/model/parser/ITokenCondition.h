#pragma once

#include <memory>
#include "ITokenConditionVerifier.h"

namespace c4 {
    namespace model {
        namespace parser {
            class ITokenCondition {
            public:
                virtual ~ITokenCondition() { }
                virtual std::unique_ptr<ITokenConditionVerifier> createVerifier() const = 0;
            };
        }
    }
}
