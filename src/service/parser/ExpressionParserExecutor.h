#pragma once

#include <memory>
#include "../../model/parser/lr/IStateHandlerVisitor.h"
#include "../../model/token/Token.h"

namespace c4 {
    namespace service {
        namespace parser {
            class ExpressionParser;

            class ExpressionParserExecutor :
                public model::parser::lr::IStateHandlerVisitor {
            public:
                ExpressionParserExecutor(
                    ExpressionParser &parser,
                    std::shared_ptr<const model::token::Token> token
                );

                ~ExpressionParserExecutor() { }

                bool hasShifted();
                bool isAccepting();

                void visit(
                    const model::parser::lr::AcceptingStateHandler &handler
                ) override;

                void visit(
                    const model::parser::lr::ReducingStateHandler &handler
                ) override;
                
                void visit(
                    const model::parser::lr::ShiftingStateHandler &handler
                ) override;

            private:
                bool accepting;
                bool executed;
                ExpressionParser &parser;
                bool shifted;
                const std::shared_ptr<const model::token::Token> token;

                void ensureAndSetExecuted();
            };
        }
    }
}