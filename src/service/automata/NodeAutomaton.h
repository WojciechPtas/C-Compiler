#include "../../model/node/Node.h"
#include "IAutomaton.h"

namespace c4 {
    namespace service {
        namespace automata {
            template <class TEdge, class TPayload>
            class NodeAutomaton : public IAutomaton<TEdge, TPayload> {
            public:
                NodeAutomaton(
                    const model::node::Node<TEdge, TPayload> &rootNode
                );

                virtual ~NodeAutomaton() { }

                std::shared_ptr<TPayload> walk(
                    io::IInputStream<TEdge>& src
                ) const = 0;

            private:
                const model::node::Node<TEdge, TPayload> &rootNode;
            };
        }
    }
}
