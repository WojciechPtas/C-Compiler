#pragma once

#include <memory>

#include "ExpressionCondition.h"
#include "ITokenCondition.h"
#include "Reduction.h"

namespace c4 {
    namespace model {
        namespace parser {
            class State {
            public:
                State();
                virtual ~State() { }

                void addJump(
                    std::shared_ptr<const ExpressionCondition> condition,
                    std::shared_ptr<const State> state
                );

                void addReduction(
                    std::shared_ptr<const ITokenCondition> condition,
                    std::shared_ptr<const Reduction> reduction
                );

                void addShift(
                    std::shared_ptr<const ITokenCondition> condition,
                    std::shared_ptr<const State> state
                );
            };
        }
    }
}
