#include <algorithm>
#include <stdexcept>
#include <cstdio>

#include "../../../debug.h"
#include "../../../util/token/KeywordUtilities.h"
#include "../../../util/token/PunctuatorUtilities.h"
#include "State.h"

using namespace c4::model::parser::lr;
using namespace c4::util::token;
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
    DBGOUT_E(
        "lr-parser",
        "(State %p, T=%2hhx, K=%11llx, P=%15llx) Installing pseudo reduction",
        this,
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask
    );

    this->decomposeCondition(
        condition,
        [&](EncounterHandler &dst) {
            if (dst.type != EncounterHandlerType::None) {
                throw logic_error("Handler installed already");
            }

            dst.reduction.expressionReduction.consumedExpressions = 0;
            dst.reduction.expressionReduction.consumedStates = consumedStates;
            dst.type = EncounterHandlerType::Reduction;
        }
    );
}

static shared_ptr<const c4::model::expression::IExpression> __test__(
    vector<shared_ptr<const c4::model::expression::IExpression>> consumed
) {
    return nullptr;
}

static unique_ptr<const c4::model::expression::IExpression> __test2__(
    const c4::model::token::Token &token
) {
    return nullptr;
}

void State::addReduction(
    LookaheadCondition condition,
    uint32_t consumedStates,
    uint32_t consumedExpressions,
    Reduction reduction
) {
    DBGOUT_E(
        "lr-parser",
        "(State %p, T=%2hhx, K=%11llx, P=%15llx) Installing reduction",
        this,
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask
    );

    this->decomposeCondition(
        condition,
        [&](EncounterHandler &dst) {
            if (dst.type != EncounterHandlerType::None) {
                throw logic_error("Handler installed already");
            }

            dst.reduction.expressionReduction.consumedExpressions =
                consumedExpressions;

            dst.reduction.expressionReduction.consumedStates = consumedStates;
            dst.type = EncounterHandlerType::Reduction;
        }
    );
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState
) {
    DBGOUT_E(
        "lr-parser",
        "(State %p, T=%2hhx, K=%11llx, P=%15llx) Installing shift",
        this,
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask
    );

    this->decomposeCondition(
        condition,
        [&](EncounterHandler &dst) {
            if (dst.type != EncounterHandlerType::None) {
                throw logic_error("Handler installed already");
            }
            
            dst.nextState = nextState;
            dst.type = EncounterHandlerType::Shift;
        }
    );
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState,
    TokenReduction reduction
) {
    DBGOUT_E(
        "lr-parser",
        "(State %p, T=%2hhx, K=%11llx, P=%15llx) Installing shift with "
        "reduction",
        this,
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask
    );

    this->decomposeCondition(
        condition,
        [&](EncounterHandler &dst) {
            if (dst.type != EncounterHandlerType::None) {
                throw logic_error("Handler installed already");
            }
            
            dst.nextState = nextState;
            dst.reduction.tokenReduction = reduction;
            dst.type =
                EncounterHandlerType::Reduction | EncounterHandlerType::Shift;
        }
    );
}

template<typename InstallFn>
void State::decomposeCondition(
    LookaheadCondition condition,
    InstallFn installFn
) {
    auto typeMask = condition.typeMask;

    if ((typeMask & TokenType::Constant) == TokenType::Constant) {
        installFn(this->encounterConstant);
    }

    if ((typeMask & TokenType::End) == TokenType::End) {
        installFn(this->encounterEnd);
    }

    if ((typeMask & TokenType::Error) == TokenType::Error) {
        installFn(this->encounterError);
    }

    if ((typeMask & TokenType::Identifier) == TokenType::Identifier) {
        installFn(this->encounterIdentifier);
    }

    if ((typeMask & TokenType::Keyword) == TokenType::Keyword) {
        auto keywords = decompose(condition.keywordMask);

        for (auto keyword : *keywords) {
            installFn(this->encounterKeyword[keyword]);
        }
    }

    if ((typeMask & TokenType::Punctuator) == TokenType::Punctuator) {
        auto punctuators = decompose(condition.punctuatorMask);

        for (auto punctuator : *punctuators) {
            installFn(this->encounterPunctuator[punctuator]);
        }
    }
}
