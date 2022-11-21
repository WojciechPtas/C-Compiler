#include "NodeAutomaton.h"

using namespace c4::model::node;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace std;

template <class TEdge, class TPayload>
NodeAutomaton<TEdge, TPayload>::NodeAutomaton(
    shared_ptr<Node<TEdge, TPayload>> rootNode
) : rootNode(rootNode) { }

template <class TEdge, class TPayload>
shared_ptr<Node<TEdge, TPayload>> NodeAutomaton<TEdge, TPayload>::walk(
    IInputStream<TEdge>& src
) const {
    shared_ptr<Node<TEdge, TPayload>> curState = this->rootNode;
    TEdge curElement;
    
    src.pushMark();

    while (src.read(&curElement)) {
        shared_ptr<Node<TEdge, TPayload>> newState = curState->getChildNode(
            curElement
        );

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
