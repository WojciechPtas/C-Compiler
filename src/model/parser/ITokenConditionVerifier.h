#pragma once

#include "../../util/token/ITokenVisitor.h"

namespace c4 {
    namespace model {
        namespace parser {
            class ITokenConditionVerifier : public util::token::ITokenVisitor {
            public:
                virtual ~ITokenConditionVerifier() { }
                virtual bool isApplying() const = 0;
            };
        }
    }
}
