#pragma once

#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class ConditionalReduction : public model::parser::PureReduction {
            public:
                ConditionalReduction();
                virtual ~ConditionalReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    const std::vector<std::shared_ptr<const model::expression::IExpression>> &consumedExpressions
                ) const override;
            };
        }
    }
}
