#include <stdexcept>

#include "../../model/expression/IExpression.h"
#include "../../model/parser/lr/ReducingStateHandler.h"
#include "../../model/parser/lr/ShiftingStateHandler.h"
#include "../../debug.h"
#include "ExpressionParser.h"
#include "ExpressionParserExecutor.h"
#include <memory>
#include "../../util/parser/lr/LookaheadTokenUtilities.h"
#include "../../model/expression/SizeOfType.h"
#include "../LLparser/LLParser.h"


using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::parser;
using namespace c4::util::parser;
using namespace std;

ExpressionParserExecutor::ExpressionParserExecutor(
    ExpressionParser &parser,
    shared_ptr<const Token> token,
    c4::service::io::IBufferedInputStream<shared_ptr<Token>> &input
) : accepting(false),
    executed(false),
    parser(parser),
    shifted(false),
    error(false),
    LLShifted(false),
    token(token),
    input(input)
    { }

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

//Resets to false immediately after
bool ExpressionParserExecutor::isError() {
    bool temp = error;
    error = false;
    return temp;
}

//Resets to false immediately after
bool ExpressionParserExecutor::hasLLShifted() {
    bool temp = LLShifted;
    LLShifted = false;
    return temp;
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

    //Exectutes and terminates if it finds a sizeof(type-name)
    if(isSizeOf(token)) {
        //Don't add any more tokens to the tokens stack. First token of sizeof(type) is sizeof, which has just been pushed
        input.pushMark();
        bool sizeOfTypeCondition;
        std::shared_ptr<Token> temp;

        sizeOfTypeCondition = 
            input.read(&temp) &&
            isLeftParenthesis(temp);
        
        if(sizeOfTypeCondition) {
            input.pushMark();
            sizeOfTypeCondition = 
                input.read(&temp) &&
                isTypeSpecifier(temp);
            input.resetAndPopMark();
        }
        if(sizeOfTypeCondition) {
            //read parenthesized type and push into expression stack
            std::shared_ptr<model::declaration::TypeName> type_name = parser.llparser->parseTypeName();
            if(type_name == nullptr) {
                //LLparser error
                parser._lastTokenRead = parser.llparser->lastTokenRead();
                error = true;
            }
            else if(!(input.read(&parser._lastTokenRead) && isRightParenthesis(parser._lastTokenRead))) {
                //LR parser error
                error = true;
            }
            else { //All good, generate the expression, put it onto the stack
                LLShifted = true;
                std::shared_ptr<const Token> sizeOfToken = parser.tokens.back();
                parser.tokens.pop_back();

                std::shared_ptr<const SizeOfType> sizeOfExpr = std::make_shared<const SizeOfType>(type_name, sizeOfToken);
                parser.expressions.push_back(sizeOfExpr);
            }
            this->shifted = true;
            input.popMark();
            return;
        }
        else {
            input.resetToMark(); //resets to after reading sizeOf
        }
        input.popMark();
    }
    //sizeof(type-name) not found
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
    this->parser.states.push_back(handler.nextState);

    DBGOUT_E(
        "parser",
        "Pushing state: '%s'",
        handler.nextState.lock()->name.c_str()
    );

    this->shifted = true;
}

void ExpressionParserExecutor::ensureAndSetExecuted() {
    if (this->executed) {
        throw logic_error("Executed already");
    }

    this->executed = true;
}
