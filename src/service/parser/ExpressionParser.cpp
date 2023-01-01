#include <stdexcept>

#include "../../debug.h"
#include "ExpressionParser.h"

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::parser;
using namespace std;

ExpressionParser::ExpressionParser(weak_ptr<const State> initialState) {
    this->states.push_back(initialState);
}

shared_ptr<const IExpression> ExpressionParser::parse(
    IInputStream<shared_ptr<const Token>> &input
) {
    shared_ptr<const Token> token;
    bool eofReached, readNext = true;
    size_t stateCount = 0, newStateCount = this->states.size();

    do {
        auto isGoto = stateCount > newStateCount;
        stateCount = newStateCount;

        if (readNext) {
            eofReached = !input.read(&token);
        }

        if (this->states.empty()) {
            throw logic_error("No state left!");
        }

        auto currentState = this->states.back().lock();

        DBGOUT_E(
            "parser",
            "Parser in state: '%s'",
            currentState->name.c_str()
        );

        DBGOUT_E("parser", "File: '%s'", token->position.file.c_str());

        if (isGoto) {
            if (this->expressions.empty()) {
                throw logic_error("No expression available!");
            }

            auto lastExpression = this->expressions.back();
            auto nextState = currentState->getGotoState(*lastExpression);

            this->states.push_back(nextState);
        } else {
            shared_ptr<const StateHandler> handler;

            if (eofReached) {
                handler = currentState->getEndHandler();
            } else {
                handler = currentState->getHandler(*token);
            }

            if (handler == nullptr) {
                throw logic_error("No handler!");
            }

            ExpressionParserExecutor executor(*this, token);
            handler->accept(executor);
            readNext = executor.hasShifted();
        }

        newStateCount = this->states.size();
    } while (!eofReached || stateCount != newStateCount);

    if (this->expressions.empty()) {
        throw logic_error("No expression available!");
    }

    return this->expressions.back();
}
