#include <stdexcept>

#include "../../model/expression/IExpression.h"
#include "ExpressionParser.h"
#include "ExpressionParserExecutor.h"

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::parser;
using namespace std;

ExpressionParserExecutor::ExpressionParserExecutor(
    ExpressionParser &parser,
    shared_ptr<const Token> token
) : executed(false), parser(parser), token(token) { }

bool ExpressionParserExecutor::hasShifted() {
    if (!this->executed) {
        throw logic_error("Not yet executed");
    }

    return this->shifted;
}

void ExpressionParserExecutor::visit(const ReducingStateHandler &handler) {
    this->ensureAndSetExecuted();

    for (uint32_t i = handler.consumedStates; i > 0; i--) {
        this->parser.states.pop_back();
    }

    vector<shared_ptr<const IExpression>> consumedExpressions;

    for (uint32_t i = handler.consumedExpressions; i > 0; i--) {
        consumedExpressions.insert(
            consumedExpressions.begin(),
            this->parser.expressions.back()
        );

        this->parser.expressions.pop_back();
    }

    this->parser.expressions.push_back(handler.executor(consumedExpressions));
}
                
void ExpressionParserExecutor::visit(const ShiftingStateHandler &handler) {
    this->ensureAndSetExecuted();

    if (handler.tokenReduction != nullptr) {
        if (this->token == nullptr) {
            throw logic_error("token is null");
        }

        this->parser.expressions.push_back(
            handler.tokenReduction(*this->token)
        );
    }

    this->parser.states.push_back(handler.nextState);
    this->shifted = true;
}

void ExpressionParserExecutor::ensureAndSetExecuted() {
    if (this->executed) {
        throw logic_error("Executed already");
    }

    this->executed = true;
}
