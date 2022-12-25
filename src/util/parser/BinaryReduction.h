#pragma once

#include "../../model/expression/BinaryExpressionType.h"
#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class BinaryReduction : public model::parser::PureReduction {
            public:
                BinaryReduction(model::expression::BinaryExpressionType type);
                virtual ~BinaryReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    const std::vector<std::shared_ptr<const model::expression::IExpression>> &consumedExpressions
                ) const override;

            private:
                const model::expression::BinaryExpressionType type;
            };
        }
    }
}
