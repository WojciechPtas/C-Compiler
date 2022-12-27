#include "State.h"

using namespace c4::model::parser;
using namespace std;

State::State() {
}

void State::addJump(
    shared_ptr<const ExpressionCondition> condition,
    shared_ptr<const State> state
) {
    // TODO: Implementation.
}

void State::addReduction(
    shared_ptr<const ITokenCondition> condition,
    shared_ptr<const Reduction> reduction
) {
    // TODO: Implementation.
}

void State::addShift(
    shared_ptr<const ITokenCondition> condition,
    shared_ptr<const State> state
) {
    // TODO: Implementation.
}
