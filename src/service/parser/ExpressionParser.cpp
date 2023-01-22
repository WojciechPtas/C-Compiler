#include <stdexcept>
#include <iostream>
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

//Returns nullptr in case of failure
shared_ptr<const IExpression> ExpressionParser::parse(
    IInputStream<shared_ptr<Token>> &input
) {
    shared_ptr<Token> token;
    bool accepting, eofReached, readNext = true;
    size_t stateCount = 0, newStateCount = this->states.size();
    ////std::cout<<"Parsing\n";
    do {
        bool isGoto = stateCount > newStateCount; //we reduced last iteration
        stateCount = newStateCount;
        //std::cout<<"Read token\n";
        if (readNext) {
            eofReached = !input.read(&token);
        }
        //std::cout<<"Parsing\n";

        if (this->states.empty()) {
            throw logic_error("No state left!"); //should never happen
        }
        //std::cout<<"Parsing\n" <<this->states.size();
        //std::cout<<"Parsing\n";
        auto currentState = this->states.back().lock();

        DBGOUT_E(
            "parser",
            "Parser in state: '%s'",
            currentState->name.c_str()
        );
        //std::cout<<"Parsing\n";

        if (isGoto) {
            if (this->expressions.empty()) {
                throw logic_error("No expression available!");
            }

            auto lastExpression = this->expressions.back();
            auto nextState = currentState->getGotoState(*lastExpression);

            this->states.push_back(nextState);
            accepting = false;
        } else {
            shared_ptr<const StateHandler> handler;

            if (eofReached) {
                handler = currentState->getEndHandler();
            } else {
                handler = currentState->getHandler(*token);
            }

            if (handler == nullptr) {
                //resetting
                std::weak_ptr<const State> initialState = states[0];
                this->states.clear();
                this->states.push_back(initialState);
                this->expressions.clear();
                this->expressions.push_back(nullptr);
            }

            ExpressionParserExecutor executor(*this, token);
            handler->accept(executor);
            readNext = executor.hasShifted();
            accepting = executor.isAccepting();
        }

        newStateCount = this->states.size();
    } while (!eofReached || !accepting); //terminates when eofReached && accepting

    if (this->expressions.empty()) {
        throw logic_error("No expression available!");
    }

    shared_ptr<const IExpression> finalExpression = this->expressions.back();
    expressions.pop_back();
    return finalExpression;
}
