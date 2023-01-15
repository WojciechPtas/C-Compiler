#pragma once

#include <memory>

#include "State.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class AcceptingStateHandler : public StateHandler {
                public:
                    AcceptingStateHandler() { }
                    ~AcceptingStateHandler() { }

                    void accept(IStateHandlerVisitor &visitor) const override {
                        visitor.visit(*this);
                    }
                };
            }
        }
    }
}
