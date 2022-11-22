#include "IAutomaton.h"

namespace c4 {
    namespace service {
        namespace automata {
            template <class TEdge, class TPayload>
            class NodeAutomaton : public IAutomaton<TEdge, TPayload> {
            public:
                NodeAutomaton(
                    std::shared_ptr<const model::node::Node<TEdge, TPayload>> rootNode
                ) : rootNode(rootNode) { }

                virtual ~NodeAutomaton() { }

                std::shared_ptr<const model::node::Node<TEdge, TPayload>> walk(
                    io::IInputStream<TEdge>& src
                ) const {
                    auto curState = this->rootNode;
                    TEdge curElement;
                    
                    src.pushMark();
                    
                    while (src.read(&curElement)) {
                        auto newState = curState->getChildNode(curElement);

                        if (!newState) {
                            src.resetToMark();
                            break;
                        }
                        
                        curState = newState;
                        src.popMark();
                    }
                    
                    src.popMark();
                    return curState;
                }

            private:
                const std::shared_ptr<const model::node::Node<TEdge, TPayload>> rootNode;
            };
        }
    }
}
