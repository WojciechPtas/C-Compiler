#include <algorithm>
#include <stdexcept>

#include "../../../debug.h"
#include "../../../util/token/KeywordUtilities.h"
#include "../../../util/token/PunctuatorUtilities.h"

#include "ReducingStateHandler.h"
#include "ShiftingStateHandler.h"
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

    this->installHandler(
        condition,
        make_unique<ReducingStateHandler>(consumedStates)
    );
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

    this->installHandler(
        condition,
        make_unique<ReducingStateHandler>(
            consumedStates,
            consumedExpressions,
            reduction
        )
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

    this->installHandler(
        condition,
        make_unique<ShiftingStateHandler>(nextState)
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

    this->installHandler(
        condition,
        make_unique<ShiftingStateHandler>(nextState, reduction)
    );
}

void State::installHandler(
    LookaheadCondition condition,
    std::unique_ptr<StateHandler> handler
) {
    auto typeMask = condition.typeMask;

    if ((typeMask & TokenType::Constant) == TokenType::Constant) {
        if (this->encounterConstant != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterConstant = move(handler);
    }

    if ((typeMask & TokenType::End) == TokenType::End) {
        if (this->encounterEnd != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterEnd = move(handler);
    }

    if ((typeMask & TokenType::Error) == TokenType::Error) {
        if (this->encounterError != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterError = move(handler);
    }

    if ((typeMask & TokenType::Identifier) == TokenType::Identifier) {
        if (this->encounterIdentifier != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterIdentifier = move(handler);
    }

    if ((typeMask & TokenType::Keyword) == TokenType::Keyword) {
        auto keywords = decompose(condition.keywordMask);

        for (auto keyword : *keywords) {
            if (this->encounterKeyword[keyword] != nullptr) {
                throw logic_error("Handler installed already");
            }

            this->encounterKeyword[keyword] = move(handler);
        }
    }

    if ((typeMask & TokenType::Punctuator) == TokenType::Punctuator) {
        auto punctuators = decompose(condition.punctuatorMask);

        for (auto punctuator : *punctuators) {
            if (this->encounterPunctuator[punctuator] != nullptr) {
                throw logic_error("Handler installed already");
            }

            this->encounterPunctuator[punctuator] = move(handler);
        }
    }
}
