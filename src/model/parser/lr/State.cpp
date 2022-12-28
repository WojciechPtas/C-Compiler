#include "State.h"

using namespace c4::model::parser::lr;
using namespace std;

void State::addAccept(LookaheadCondition condition) {
    // TODO: Implementation.
}

void State::addJump(
    ExpressionCondition condition,
    weak_ptr<const State> nextState
) {
    // TODO: Implementation.
}

void State::addReduction(
    LookaheadCondition condition,
    uint32_t consumedStates
) {
    // TODO: Implementation.
}

void State::addReduction(
    LookaheadCondition condition,
    uint32_t consumedStates,
    uint32_t consumedExpressions,
    Reduction reduction
) {
    // TODO: Implementation.
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState
) {
    // TODO: Implementation.
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState,
    TokenReduction reduction
) {
    // TODO: Implementation.
}
