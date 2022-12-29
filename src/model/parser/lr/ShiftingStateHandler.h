#pragma once

#include <memory>

#include "State.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class ShiftingStateHandler : public StateHandler {
                public:
                    ShiftingStateHandler(
                        std::weak_ptr<const State> nextState
                    ) : nextState(nextState), tokenReduction(nullptr) { }

                    ShiftingStateHandler(
                        std::weak_ptr<const State> nextState,
                        TokenReduction tokenReduction
                    ) : nextState(nextState),
                        tokenReduction(tokenReduction) { }
                    
                    ~ShiftingStateHandler() { }

                private:
                    const std::weak_ptr<const State> nextState;
                    const TokenReduction tokenReduction;
                };
            }
        }
    }
}
