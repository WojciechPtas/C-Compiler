#pragma once

#include "IStateHandlerVisitor.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class StateHandler {
                public:
                    virtual ~StateHandler() { }
                    virtual void accept(
                        IStateHandlerVisitor &visitor
                    ) const = 0;
                };
            }
        }
    }
}
