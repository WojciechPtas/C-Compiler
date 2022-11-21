#pragma once

#include <memory>
#include "../io/IInputStream.h"

namespace c4 {
    namespace service {
        namespace automata {
            template <class TEdge, class TPayload>
            class IAutomaton {
            public:
                virtual std::shared_ptr<TPayload> walk(
                    io::IInputStream<TEdge>& src
                ) const = 0;
            };
        }
    }
}
