#pragma once
#include "io/IInputStream.h"
#include <memory>
#include "io/Node.h"

namespace c4 {
    namespace service {
        namespace automata {
            template <class TEdge, class TPayload>
            class IAutomaton {
            
            public:
                std::shared_ptr<TPayload> walk(io::IInputStream<TEdge>& src) const {
                    //TO BE IMPLEMENTED
                    return nullptr;
                }
            };
        }
    }
}