#pragma once

#include <memory>
#include <vector>

#include "../../model/expression/IExpression.h"
#include "../../model/parser/lr/State.h"
#include "../../model/token/Token.h"
#include "../io/IBufferedInputStream.h"
#include "ExpressionParserExecutor.h"

namespace c4 {
    namespace service {
        namespace parser {
            class LLParser; //Avoid circular include
            class ExpressionParser {
                friend ExpressionParserExecutor;

            public:
                ExpressionParser(
                    std::weak_ptr<const model::parser::lr::State> initialState,
                    LLParser* llparser
                );

                ~ExpressionParser() { }

                std::shared_ptr<const model::expression::IExpression> parse(
                    io::IBufferedInputStream<std::shared_ptr<model::token::Token>> &input
                );

                //Guaranteed to be non-garbage only in case of parsing failure
                std::shared_ptr<model::token::Token> lastTokenRead() {
                    return _lastTokenRead;
                }

            private:
                std::weak_ptr<const model::parser::lr::State> initialState;
                std::shared_ptr<model::token::Token> _lastTokenRead;
                std::vector<std::shared_ptr<const model::expression::IExpression>> expressions;
                std::vector<std::weak_ptr<const model::parser::lr::State>> states;
                std::vector<std::shared_ptr<const model::token::Token>> tokens;
                LLParser* llparser;

                void reset();
            };
        }
    }
}
