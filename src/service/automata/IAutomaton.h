#pragma once

#include <memory>

#include "../../model/node/Node.h"
#include "../io/IInputStream.h"

namespace c4 {
    namespace service {
        namespace automata {
            template <class TEdge, class TPayload>
            class IAutomaton {
            public:
                virtual std::shared_ptr<const model::node::Node<TEdge, TPayload>> walk(
                    io::IInputStream<TEdge>& src
                ) const = 0;
            };
        }
    }
}
