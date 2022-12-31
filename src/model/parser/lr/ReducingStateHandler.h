#pragma once

#include <cstdint>
#include <memory>

#include "State.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class ReducingStateHandler : public StateHandler {
                public:
                    const std::uint32_t consumedStates;
                    const std::uint32_t consumedExpressions;
                    const Reduction executor;

                    ReducingStateHandler(std::uint32_t consumedStates) :
                        consumedStates(consumedStates),
                        consumedExpressions(0),
                        executor(nullptr) { }

                    ReducingStateHandler(
                        std::uint32_t consumedStates,
                        std::uint32_t consumedExpressions,
                        Reduction executor
                    ) : consumedStates(consumedStates),
                        consumedExpressions(consumedExpressions),
                        executor(executor) { }

                    ~ReducingStateHandler() { }

                    void accept(IStateHandlerVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
            }
        }
    }
}
