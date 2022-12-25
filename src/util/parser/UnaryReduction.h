#pragma once

#include "../../model/expression/UnaryExpressionType.h"
#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class UnaryReduction : public model::parser::PureReduction {
            public:
                UnaryReduction(model::expression::UnaryExpressionType type);
                virtual ~UnaryReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    const std::vector<std::shared_ptr<const model::expression::IExpression>> &consumedExpressions
                ) const override;

            private:
                const model::expression::UnaryExpressionType type;
            };
        }
    }
}
