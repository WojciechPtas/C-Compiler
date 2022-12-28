#include "State.h"

using namespace c4::model::parser::lr;
using namespace std;

void State::addJump(
    ExpressionCondition condition,
    shared_ptr<const State> nextState
) {
    // TODO: Implementation.
}

void State::addReduction(
    LookaheadCondition condition,
    shared_ptr<const Reduction> reduction
) {
    // TODO: Implementation.
}

void State::addShift(
    LookaheadCondition condition,
    shared_ptr<const State> nextState
) {
    // TODO: Implementation.
}
