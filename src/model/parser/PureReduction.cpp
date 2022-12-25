#include "PureReduction.h"

using namespace c4::model::parser;
using namespace std;

PureReduction::PureReduction(
    uint32_t stateConsumption,
    uint32_t expressionConsumption
) : Reduction(stateConsumption, expressionConsumption) { }
