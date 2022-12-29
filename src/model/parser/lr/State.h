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

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class State;

                typedef std::function<std::shared_ptr<const expression::IExpression>(std::vector<std::shared_ptr<const expression::IExpression>>)> Reduction;
                typedef std::function<std::unique_ptr<const expression::IExpression>(const token::Token&)> TokenReduction;

                enum class EncounterHandlerType : unsigned char {
                    None        = 0x00,
                    Reduction   = 0x01,
                    Shift       = 0x02
                };

                inline EncounterHandlerType operator| (
                    EncounterHandlerType l,
                    EncounterHandlerType r
                ) {
                    return EncounterHandlerType(
                        std::underlying_type<EncounterHandlerType>::type(l) |
                        std::underlying_type<EncounterHandlerType>::type(r)
                    );
                }

                struct EncounterHandler {
                    EncounterHandlerType type;

                    union _reduction {
                        struct {
                            std::uint32_t consumedStates;
                            std::uint32_t consumedExpressions;
                            Reduction executor;
                        } expressionReduction;

                        TokenReduction tokenReduction;

                        _reduction() { }
                        ~_reduction() { }
                    } reduction;

                    std::weak_ptr<const State> nextState;

                    EncounterHandler() {
                        this->type = EncounterHandlerType::None;
                    }
                };

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
                    EncounterHandler encounterConstant;
                    EncounterHandler encounterEnd;
                    EncounterHandler encounterError;
                    EncounterHandler encounterIdentifier;
                    std::map<token::Keyword, EncounterHandler> encounterKeyword;
                    std::map<token::Punctuator, EncounterHandler> encounterPunctuator;

                    template<typename InstallFn>
                    void decomposeCondition(
                        LookaheadCondition condition,
                        InstallFn installFn
                    );
                };
            }
        }
    }
}
