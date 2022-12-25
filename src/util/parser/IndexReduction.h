#pragma once

#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class IndexReduction : public model::parser::PureReduction {
            public:
                IndexReduction();
                virtual ~IndexReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    const std::vector<std::shared_ptr<const model::expression::IExpression>> &consumedExpressions
                ) const override;
            };
        }
    }
}
