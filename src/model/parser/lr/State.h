#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "../../expression/IExpression.h"
#include "../../token/Token.h"

#include "ExpressionCondition.h"
#include "LookaheadCondition.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                typedef std::function<std::shared_ptr<const expression::IExpression>(std::vector<std::shared_ptr<const expression::IExpression>>)> Reduction;
                typedef std::function<std::unique_ptr<const expression::IExpression>(const token::Token&)> TokenReduction;

                class State {
                public:
                    ~State() { }

                    void addAccept(LookaheadCondition condition);
                    void addJump(
                        ExpressionCondition condition,
                        std::weak_ptr<const State> nextState
                    );

                    void addReduction(
                        LookaheadCondition condition,
                        std::uint32_t consumedStates
                    );

                    void addReduction(
                        LookaheadCondition condition,
                        std::uint32_t consumedStates,
                        std::uint32_t consumedExpressions,
                        Reduction reduction
                    );

                    void addShift(
                        LookaheadCondition condition,
                        std::weak_ptr<const State> nextState
                    );

                    void addShift(
                        LookaheadCondition condition,
                        std::weak_ptr<const State> nextState,
                        TokenReduction reduction
                    );

                private:
                    std::unique_ptr<StateHandler> encounterConstant;
                    std::unique_ptr<StateHandler> encounterEnd;
                    std::unique_ptr<StateHandler> encounterError;
                    std::unique_ptr<StateHandler> encounterIdentifier;
                    std::map<token::Keyword, std::unique_ptr<StateHandler>> encounterKeyword;
                    std::map<token::Punctuator, std::unique_ptr<StateHandler>> encounterPunctuator;

                    void installHandler(
                        LookaheadCondition condition,
                        std::unique_ptr<StateHandler> handler
                    );
                };
            }
        }
    }
}
