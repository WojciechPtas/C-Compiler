#pragma once

#include <memory>
#include <stdexcept>

#include "../../expression/BinaryExpression.h"
#include "../../expression/IExpressionVisitor.h"
#include "../../expression/MemberExpression.h"
#include "../../expression/UnaryExpression.h"
#include "../../expression/CallExpression.h"
#include "../../expression/SizeOfType.h"

#include "State.h"
#include "StateHandler.h"

namespace c4 {
    namespace model {
        namespace parser {
            namespace lr {
                class GotoFinder : public expression::IExpressionVisitor {
                public:
                    GotoFinder(const State &state) :
                        state(state),
                        executed(false) { }

                    std::weak_ptr<const State> getDeterminedState() {
                        if (!this->executed) {
                            throw std::logic_error("Not determined yet");
                        }

                        return this->determinedState;
                    }
                    
                    void visit(
                        const expression::BinaryExpression &expr
                    ) override {
                        this->checkAndSetExecuted();

                        auto entry = this->state.gotoAfterBinary.find(
                            expr.type
                        );

                        if (entry != this->state.gotoAfterBinary.end()) {
                            this->determinedState = (*entry).second;
                        }
                    }

                    void visit(
                        const expression::CallArguments &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState = this->state.gotoAfterCallArguments;
                    }

                    void visit(
                        const expression::CallExpression &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState = this->state.gotoAfterCall;
                    }

                    void visit(
                        const expression::ConditionalExpression &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState =
                            this->state.gotoAfterConditional;
                    }

                    void visit(
                        const expression::ConstantExpression &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState = this->state.gotoAfterConstant;
                    }

                    void visit(
                        const expression::IdentifierExpression &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState =
                            this->state.gotoAfterIdentifier;
                    }

                    void visit(
                        const expression::IndexExpression &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();
                        this->determinedState = this->state.gotoAfterIndex;
                    }

                    void visit(
                        const expression::MemberExpression &expr
                    ) override {
                        this->checkAndSetExecuted();

                        auto entry = this->state.gotoAfterMember.find(
                            expr.type
                        );

                        if (entry != this->state.gotoAfterMember.end()) {
                            this->determinedState = (*entry).second;
                        }
                    }

                    void visit(
                        const expression::UnaryExpression &expr
                    ) override {
                        this->checkAndSetExecuted();

                        auto entry = this->state.gotoAfterUnary.find(
                            expr.type
                        );

                        if (entry != this->state.gotoAfterUnary.end()) {
                            this->determinedState = (*entry).second;
                        }
                    }

                    void visit(
                        const expression::SizeOfType &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();

                        auto sizeOf = expression::UnaryExpressionType::Sizeof;
                        this->determinedState =
                            this->state.gotoAfterUnary.find(sizeOf)->second;
                    }

                    void visit(
                        const expression::TypeInSizeof &expr
                    ) override {
                        (void) expr;

                        this->checkAndSetExecuted();

                        this->determinedState = 
                            this->state.gotoAfterType;
                    }

                private:
                    const State &state;
                    std::weak_ptr<const State> determinedState;
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
