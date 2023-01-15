#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
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
                // Avoid circular dependencies.

                class GotoFinder;
                class StateHandlerFinder;

                // Real declarations.

                typedef std::function<std::shared_ptr<const expression::IExpression>(std::vector<std::shared_ptr<const expression::IExpression>>&)> Reduction;
                //Reduction is now type "Function taking (reference of) vector of pointers of Expression, returns pointer to Expression"
                
                typedef std::function<std::unique_ptr<const expression::IExpression>(const token::Token&)> TokenReduction;
                //TokenReduction is now type "Function taking (reference of) Token, returns (unique) pointer to Expression"

                class State {
                    friend GotoFinder;
                    friend StateHandlerFinder;

                public:
                    const std::string name;

                    State(const std::string &name);
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

                    std::shared_ptr<const StateHandler> getEndHandler() const;

                    std::weak_ptr<const State> getGotoState(
                        const expression::IExpression &expression
                    ) const;

                    std::shared_ptr<const StateHandler> getHandler(
                        const model::token::Token &token
                    ) const;

                private:
                    // Lookahead-based stuff

                    std::shared_ptr<StateHandler> encounterConstant;
                    std::shared_ptr<StateHandler> encounterEnd;
                    std::shared_ptr<StateHandler> encounterError;
                    std::shared_ptr<StateHandler> encounterIdentifier;
                    std::map<token::Keyword, std::shared_ptr<StateHandler>> encounterKeyword;
                    std::map<token::Punctuator, std::shared_ptr<StateHandler>> encounterPunctuator;

                    void installLookaheadHandler(
                        LookaheadCondition condition,
                        std::shared_ptr<StateHandler> handler
                    );

                    // Jump-based stuff

                    std::weak_ptr<const State> gotoAfterCall;
                    std::weak_ptr<const State> gotoAfterConditional;
                    std::weak_ptr<const State> gotoAfterConstant;
                    std::weak_ptr<const State> gotoAfterIdentifier;
                    std::weak_ptr<const State> gotoAfterIndex;
                    std::map<expression::BinaryExpressionType, std::weak_ptr<const State>> gotoAfterBinary;
                    std::map<expression::MemberExpressionType, std::weak_ptr<const State>> gotoAfterMember;
                    std::map<expression::UnaryExpressionType, std::weak_ptr<const State>> gotoAfterUnary;
                };
            }
        }
    }
}
