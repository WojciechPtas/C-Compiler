#pragma once

#include <memory>
#include <vector>

#include "../../model/expression/IExpression.h"
#include "../../model/parser/lr/State.h"
#include "../../model/token/Token.h"
#include "../io/IInputStream.h"
#include "ExpressionParserExecutor.h"

namespace c4 {
    namespace service {
        namespace parser {
            class ExpressionParser {
                friend ExpressionParserExecutor;

            public:
                ExpressionParser(
                    std::weak_ptr<const model::parser::lr::State> initialState
                );

                ~ExpressionParser() { }

                std::shared_ptr<const model::expression::IExpression> parse(
                    io::IInputStream<std::shared_ptr<const model::token::Token>> &input
                );

            private:
                std::vector<std::shared_ptr<const model::expression::IExpression>> expressions;
                std::vector<std::weak_ptr<const model::parser::lr::State>> states;
            };
        }
    }
}
