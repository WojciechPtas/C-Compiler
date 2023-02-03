#include <stdexcept>

#include "../../model/expression/IExpression.h"
#include "../../model/parser/lr/ReducingStateHandler.h"
#include "../../model/parser/lr/ShiftingStateHandler.h"
#include "../../debug.h"
#include "ExpressionParser.h"
#include "ExpressionParserExecutor.h"
#include <memory>

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::parser;
using namespace std;

ExpressionParserExecutor::ExpressionParserExecutor(
    ExpressionParser &parser,
    shared_ptr<const Token> token
) : accepting(false),
    executed(false),
    parser(parser),
    shifted(false),
    token(token) { }

bool ExpressionParserExecutor::hasShifted() {
    if (!this->executed) {
        throw logic_error("Not yet executed");
    }

    return this->shifted;
}

bool ExpressionParserExecutor::isAccepting() {
    if (!this->executed) {
        throw logic_error("Not yet executed");
    }

    return this->accepting;
}

void ExpressionParserExecutor::visit(const AcceptingStateHandler &handler) {
    (void) handler;

    this->ensureAndSetExecuted();
    DBGOUT("parser", "Accepting input.");
    this->accepting = true;
}

void ExpressionParserExecutor::visit(const ReducingStateHandler &handler) {
    this->ensureAndSetExecuted();

    uint32_t nConsumedTokens = handler.consumedStates - handler.consumedExpressions;

    DBGOUT_E(
        "parser",
        "Applying reduction, consuming %d states and %d expresssions",
        handler.consumedStates,
        handler.consumedExpressions
    );

    for (uint32_t i = handler.consumedStates; i > 0; i--) {
        this->parser.states.pop_back();
    }

    auto reductionFn = handler.executor;

    if (reductionFn == nullptr) {
        //In this case we had produced an expression by using the scuffed shift which produces an expression
        for (uint32_t i = handler.consumedExpressions; i > 0; i--) {
            this->parser.expressions.pop_back();
            // this->parser.tokens.pop_back();
        }
    } else {
        vector<shared_ptr<const IExpression>> consumedExpressions;

        for (uint32_t i = handler.consumedExpressions; i > 0; i--) {
            consumedExpressions.insert(
                consumedExpressions.begin(),
                this->parser.expressions.back()
            );

            this->parser.expressions.pop_back();
        }

        vector<shared_ptr<const Token>> consumedTokens(
            parser.tokens.end()-1-nConsumedTokens,
            parser.tokens.end()-1
        );
        parser.tokens.erase(
            parser.tokens.end()-1-nConsumedTokens,
            parser.tokens.end()-1
        );
            

        auto expression = reductionFn(consumedExpressions, consumedTokens);
        this->parser.expressions.push_back(expression);
    }
}
                
void ExpressionParserExecutor::visit(const ShiftingStateHandler &handler) {
    this->ensureAndSetExecuted();

    if (handler.tokenReduction != nullptr) {
        if (this->token == nullptr) {
            throw logic_error("token is null");
        }

        auto expression = handler.tokenReduction(*this->token);
        this->parser.expressions.push_back(
            expression
        );
        this->parser.tokens.pop_back();
    }

    DBGOUT_E(
        "parser",
        "Pushing state: '%s'",
        handler.nextState.lock()->name.c_str()
    );

    this->parser.states.push_back(handler.nextState);
    this->shifted = true;
}

void ExpressionParserExecutor::ensureAndSetExecuted() {
    if (this->executed) {
        throw logic_error("Executed already");
    }

    this->executed = true;
}
