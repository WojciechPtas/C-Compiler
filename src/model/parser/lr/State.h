#pragma once

#include <memory>

#include "ExpressionCondition.h"
#include "LookaheadCondition.h"
#include "Reduction.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class State {
                public:
                    ~State() { }

                    void addJump(
                        ExpressionCondition condition,
                        std::shared_ptr<const State> nextState
                    );

                    void addReduction(
                        LookaheadCondition condition,
                        std::shared_ptr<const Reduction> reduction
                    );

                    void addShift(
                        LookaheadCondition condition,
                        std::shared_ptr<const State> nextState
                    );
                };
            }
        }
    }
}
