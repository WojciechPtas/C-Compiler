#include "Reduction.h"

using namespace c4::model::parser;
using namespace std;

Reduction::Reduction(
    uint32_t stateConsumption,
    uint32_t expressionConsumption
) : stateConsumption(stateConsumption), expressionConsumption(
    expressionConsumption
) { }
