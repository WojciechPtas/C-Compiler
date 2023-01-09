#include "../../debug.h"
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
                    io::IBufferedInputStream<TEdge>& src
                ) const {
                    DBGOUT("automata", "Entering walk");

                    auto curState = this->rootNode;
                    std::shared_ptr<const model::node::Node<TEdge, TPayload>> acceptingState =
                        nullptr;
                    TEdge curElement;
                    
                    src.pushMark();
                    
                    while (src.read(&curElement)) {
                        auto newState = curState->getChildNode(curElement);

                        if (!newState) {
                            break;
                        }
                        
                        curState = newState;

                        if (curState->getResult() != nullptr) {
                            acceptingState = curState;
                            src.moveMark();
                        }
                    }
                    
                    src.resetAndPopMark();
                    DBGOUT("automata", "Leaving walk");
                    return acceptingState;
                }

            private:
                const std::shared_ptr<const model::node::Node<TEdge, TPayload>> rootNode;
            };
        }
    }
}
