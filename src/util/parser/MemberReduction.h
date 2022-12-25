#pragma once

#include "../../model/expression/MemberExpressionType.h"
#include "../../model/parser/PureReduction.h"

namespace c4 {
    namespace util {
        namespace parser {
            class MemberReduction : public model::parser::PureReduction {
            public:
                MemberReduction(model::expression::MemberExpressionType type);
                virtual ~MemberReduction() { }

                std::shared_ptr<const model::expression::IExpression> reduce(
                    const std::vector<std::shared_ptr<const model::expression::IExpression>> &consumedExpressions
                ) const override;

            private:
                const model::expression::MemberExpressionType type;
            };
        }
    }
}
