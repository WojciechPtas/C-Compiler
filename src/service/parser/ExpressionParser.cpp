#include <stdexcept>
#include <iostream>
#include "../../debug.h"
#include "ExpressionParser.h"
#include "../LLparser/LLParser.h"
#include "../../util/token/ParserVisitor.h"
#include "../../model/declaration/TypeName.h"
#include "../../model/expression/SizeOfType.h"

using namespace c4::model::expression;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::parser;
using namespace std;

ExpressionParser::ExpressionParser(weak_ptr<const State> initialState, LLParser* llparser)
: initialState(initialState), llparser(llparser) {
    this->states.push_back(initialState);
}

bool isSizeOf(std::shared_ptr<const Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::keyword &&
        pv.getSepcificValue().k == Keyword::Sizeof
    );
}

bool isTypeSpecifier(std::shared_ptr<const Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::keyword && (
            pv.getSepcificValue().k == Keyword::Char ||
            pv.getSepcificValue().k == Keyword::Double ||
            pv.getSepcificValue().k == Keyword::Float ||
            pv.getSepcificValue().k == Keyword::Int ||
            pv.getSepcificValue().k == Keyword::Long ||
            pv.getSepcificValue().k == Keyword::Short ||
            pv.getSepcificValue().k == Keyword::Signed ||
            pv.getSepcificValue().k == Keyword::Struct ||
            pv.getSepcificValue().k == Keyword::Unsigned ||
            pv.getSepcificValue().k == Keyword::Void ||
            pv.getSepcificValue().k == Keyword::__Bool
        )
    );
}

bool isLeftParenthesis(std::shared_ptr<const Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::punctuator && (
            pv.getSepcificValue().p == Punctuator::LeftParenthesis
        )
    );
}

bool isRightParenthesis(std::shared_ptr<const Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::punctuator && (
            pv.getSepcificValue().p == Punctuator::RightParenthesis
        )
    );
}

void ExpressionParser::reset() {
    this->tokens.clear();
    this->states.clear();
    this->states.push_back(initialState);
    this->expressions.clear();
    this->expressions.push_back(nullptr);
}


//Returns nullptr in case of failure
shared_ptr<const IExpression> ExpressionParser::parse(
    IBufferedInputStream<shared_ptr<Token>> &input
) {
    shared_ptr<Token> convertedToken; //Will store token->convertDigraph() here
    bool accepting, eofReached, readNext = true;
    size_t stateCount = 0, newStateCount = this->states.size();
    
    do {
        bool isGoto = stateCount > newStateCount; //we reduced last iteration
        stateCount = newStateCount;
        if (readNext) { //I shifted last time
            eofReached = !input.read(&_lastTokenRead);
            tokens.push_back(_lastTokenRead);
            convertedToken = _lastTokenRead->convertDigraph();

            if(isSizeOf(convertedToken)) {
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
                    std::shared_ptr<model::declaration::TypeName> type_name = llparser->parseTypeName();
                    if(type_name == nullptr) {
                        //LLparser error
                        _lastTokenRead = llparser->lastTokenRead();
                        reset();
                        break; //Goes to end of while: a nullptr will be returned
                    }
                    else if(!(input.read(&_lastTokenRead) && isRightParenthesis(_lastTokenRead))) {
                        //LR parser error
                        reset();
                        break; //Goes to end of while: a nullptr will be returned
                    }
                    else { //All good, generate the expression, put it onto the stack
                        isGoto = true;
                        std::shared_ptr<const Token> sizeOfToken = tokens.back();
                        tokens.pop_back();

                        std::shared_ptr<const SizeOfType> sizeOfExpr = std::make_shared<const SizeOfType>(type_name, sizeOfToken);
                        expressions.push_back(sizeOfExpr);
                    }
                }
                else { //Continue normally
                    input.resetToMark(); //resets to after reading sizeOf
                }
                input.popMark();
            }
        }

        if (this->states.empty()) {
            throw logic_error("No state left!"); //should never happen
        }
        auto currentState = this->states.back().lock();

        DBGOUT_E(
            "parser",
            "Parser in state: '%s'",
            currentState->name.c_str()
        );

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
                handler = currentState->getHandler(*convertedToken);
            }

            if (handler == nullptr) {
                reset();
                break;
            }

            ExpressionParserExecutor executor(*this, convertedToken);
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