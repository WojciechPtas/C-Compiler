#pragma once

#include <memory>
#include <stdexcept>

#include "../../token/ITokenVisitor.h"
#include "../../token/KeywordToken.h"
#include "../../token/PunctuatorToken.h"

#include "State.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class StateHandlerFinder : public token::ITokenVisitor {
                public:
                    StateHandlerFinder(const State &state) :
                        state(state),
                        executed(false) { }

                    std::shared_ptr<const StateHandler> getDeterminedHandler() {
                        if (!this->executed) {
                            throw std::logic_error("Not determined yet");
                        }

                        return this->determinedHandler;
                    }

                    void visit(const token::ConstantToken &token) override {
                        (void) token;

                        this->checkAndSetExecuted();
                        this->determinedHandler =
                            this->state.encounterConstant;
                    }

                    void visit(const token::ErrorToken &token) override {
                        (void) token;

                        this->checkAndSetExecuted();
                        this->determinedHandler = this->state.encounterError;
                    }

                    void visit(const token::IdentifierToken &token) override {
                        (void) token;

                        this->checkAndSetExecuted();
                        this->determinedHandler =
                            this->state.encounterIdentifier;
                    }

                    void visit(const token::KeywordToken &token) override {
                        this->checkAndSetExecuted();

                        auto entry = this->state.encounterKeyword.find(
                            token.keyword
                        );

                        if (entry != this->state.encounterKeyword.end()) {
                            this->determinedHandler = (*entry).second;
                        }
                    }

                    void visit(const token::PunctuatorToken &token) override {
                        this->checkAndSetExecuted();

                        auto entry = this->state.encounterPunctuator.find(
                            token.punctuator
                        );

                        if (entry != this->state.encounterPunctuator.end()) {
                            this->determinedHandler = (*entry).second;
                        }
                    }

                private:
                    const State &state;
                    std::shared_ptr<const StateHandler> determinedHandler;
                    bool executed;

                    void checkAndSetExecuted() {
                        if (this->executed) {
                            throw std::logic_error("Executed already");
                        }

                        this->executed = true;
                    }
                };
            }
        }
    }
}
