#pragma once

#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class ParenthesesReduction : public model::parser::PureReduction {
            public:
                ParenthesesReduction();
                virtual ~ParenthesesReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    std::shared_ptr<std::vector<std::shared_ptr<const model::expression::IExpression>>> consumedExpressions
                ) const override;
            };
        }
    }
}
