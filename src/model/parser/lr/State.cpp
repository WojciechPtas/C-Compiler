#include <algorithm>
#include <stdexcept>

#include "../../../debug.h"
#include "../../../util/expression/BinaryExpressionUtilities.h"
#include "../../../util/expression/MemberExpressionUtilities.h"
#include "../../../util/expression/UnaryExpressionUtilities.h"
#include "../../../util/token/KeywordUtilities.h"
#include "../../../util/token/PunctuatorUtilities.h"

#include "AcceptingStateHandler.h"
#include "GotoFinder.h"
#include "ReducingStateHandler.h"
#include "ShiftingStateHandler.h"
#include "State.h"
#include "StateHandlerFinder.h"

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::util::expression;
using namespace c4::util::token;
using namespace std;

State::State(const std::string &name) : name(name) { }

void State::addAccept(LookaheadCondition condition) {
    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, K=%11llx, P=%15llx) Installing accept [S='%s']",
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask,
        this->name.c_str()
    );

    auto handler = make_shared<AcceptingStateHandler>();
    this->installLookaheadHandler(condition, handler);
}

void State::addJump(
    ExpressionCondition condition,
    weak_ptr<const State> nextState
) {
    auto typeMask = condition.typeMask;

    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, B=%3hx, M=%2hhx, U=%3hhx) Installing jump [S='%s']",
        condition.typeMask,
        condition.binaryExpressionMask,
        condition.memberExpressionMask,
        condition.unaryExpressionMask,
        this->name.c_str()
    );

    if ((typeMask & ExpressionType::Binary) == ExpressionType::Binary) {
        auto operators = decompose(condition.binaryExpressionMask);

        for (auto op : *operators) {
            if (!this->gotoAfterBinary[op].expired()) {
                throw logic_error("Goto installed already");
            }

            this->gotoAfterBinary[op] = nextState;
        }
    }

    if ((typeMask & ExpressionType::Call) == ExpressionType::Call) {
        if (!this->gotoAfterCall.expired()) {
            throw logic_error("Goto installed already");
        }

        this->gotoAfterCall = nextState;
    }

    if ((typeMask & ExpressionType::Conditional) == ExpressionType::Conditional) {
        if (!this->gotoAfterConditional.expired()) {
            throw logic_error("Goto installed already");
        }

        this->gotoAfterConditional = nextState;
    }

    if ((typeMask & ExpressionType::Constant) == ExpressionType::Constant) {
        if (!this->gotoAfterConstant.expired()) {
            throw logic_error("Goto installed already");
        }
        
        this->gotoAfterConstant = nextState;
    }

    if ((typeMask & ExpressionType::Identifier) == ExpressionType::Identifier) {
        if (!this->gotoAfterIdentifier.expired()) {
            throw logic_error("Goto installed already");
        }
        
        this->gotoAfterIdentifier = nextState;
    }

    if ((typeMask & ExpressionType::Index) == ExpressionType::Index) {
        if (!this->gotoAfterIndex.expired()) {
            throw logic_error("Goto installed already");
        }

        this->gotoAfterIndex = nextState;
    }

    if ((typeMask & ExpressionType::Member) == ExpressionType::Member) {
        auto accessTypes = decompose(condition.memberExpressionMask);

        for (auto accessType : *accessTypes) {
            if (!this->gotoAfterMember[accessType].expired()) {
                throw logic_error("Goto installed already");
            }

            this->gotoAfterMember[accessType] = nextState;
        }
    }

    if ((typeMask & ExpressionType::Unary) == ExpressionType::Unary) {
        auto operators = decompose(condition.unaryExpressionMask);

        for (auto op : *operators) {
            if (!this->gotoAfterUnary[op].expired()) {
                throw logic_error("Goto installed already");
            }

            this->gotoAfterUnary[op] = nextState;
        }
    }
}

void State::addReduction(
    LookaheadCondition condition,
    uint32_t consumedStates
) {
    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, K=%11llx, P=%15llx) Installing pseudo reduction [S='%s']",
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask,
        this->name.c_str()
    );

    auto handler = make_shared<ReducingStateHandler>(consumedStates);
    this->installLookaheadHandler(condition, handler);
}

void State::addReduction(
    LookaheadCondition condition,
    uint32_t consumedStates,
    uint32_t consumedExpressions,
    Reduction reduction
) {
    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, K=%11llx, P=%15llx) Installing reduction [S='%s']",
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask,
        this->name.c_str()
    );

    auto handler = make_shared<ReducingStateHandler>(
        consumedStates,
        consumedExpressions,
        reduction
    );

    this->installLookaheadHandler(condition, handler);
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState
) {
    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, K=%11llx, P=%15llx) Installing shift [S='%s']",
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask,
        this->name.c_str()
    );

    auto handler = make_shared<ShiftingStateHandler>(nextState);
    this->installLookaheadHandler(condition, handler);
}

void State::addShift(
    LookaheadCondition condition,
    weak_ptr<const State> nextState,
    TokenReduction reduction
) {
    DBGOUT_E(
        "lr-parser",
        "(T=%2hhx, K=%11llx, P=%15llx) Installing shift with reduction "
        "[S='%s']",
        condition.typeMask,
        condition.keywordMask,
        condition.punctuatorMask,
        this->name.c_str()
    );

    auto handler = make_shared<ShiftingStateHandler>(nextState, reduction);
    this->installLookaheadHandler(condition, handler);
}

shared_ptr<const StateHandler> State::getEndHandler() const {
    return this->encounterEnd;
}

weak_ptr<const State> State::getGotoState(
    const IExpression &expression
) const {
    GotoFinder finder(*this);
    expression.accept(finder);
    return finder.getDeterminedState();
}

shared_ptr<const StateHandler> State::getHandler(
    const model::token::Token &token
) const {
    StateHandlerFinder finder(*this);
    token.accept(finder);
    return finder.getDeterminedHandler();
}

void State::installLookaheadHandler(
    LookaheadCondition condition,
    std::shared_ptr<StateHandler> handler
) {
    auto typeMask = condition.typeMask;

    if ((typeMask & TokenType::Constant) == TokenType::Constant) {
        if (this->encounterConstant != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterConstant = handler;
    }

    if ((typeMask & TokenType::End) == TokenType::End) {
        if (this->encounterEnd != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterEnd = handler;
    }

    if ((typeMask & TokenType::Error) == TokenType::Error) {
        if (this->encounterError != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterError = handler;
    }

    if ((typeMask & TokenType::Identifier) == TokenType::Identifier) {
        if (this->encounterIdentifier != nullptr) {
            throw logic_error("Handler installed already");
        }
        
        this->encounterIdentifier = handler;
    }

    if ((typeMask & TokenType::Keyword) == TokenType::Keyword) {
        auto keywords = decompose(condition.keywordMask);

        for (auto keyword : *keywords) {
            if (this->encounterKeyword[keyword] != nullptr) {
                throw logic_error("Handler installed already");
            }

            this->encounterKeyword[keyword] = handler;
        }
    }

    if ((typeMask & TokenType::Punctuator) == TokenType::Punctuator) {
        auto punctuators = decompose(condition.punctuatorMask);

        for (auto punctuator : *punctuators) {
            if (this->encounterPunctuator[punctuator] != nullptr) {
                throw logic_error("Handler installed already");
            }

            this->encounterPunctuator[punctuator] = handler;
        }
    }
}
