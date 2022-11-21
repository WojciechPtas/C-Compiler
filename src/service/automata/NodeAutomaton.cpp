#include "NodeAutomaton.h"

using namespace c4::model::node;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace std;

template <class TEdge, class TPayload>
NodeAutomaton<TEdge, TPayload>::NodeAutomaton(
    const Node<TEdge, TPayload> &rootNode
) {
}

template <class TEdge, class TPayload>
shared_ptr<TPayload> NodeAutomaton<TEdge, TPayload>::walk(
    IInputStream<TEdge>& src
) const {
    return shared_ptr<NodeAutomaton>(nullptr);
}
