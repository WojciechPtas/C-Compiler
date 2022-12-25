#pragma once

#include <memory>
#include <vector>

#include "../expression/IExpression.h"
#include "Reduction.h"

namespace c4 {
    namespace model {
        namespace parser {
            class PureReduction : public Reduction {
            public:
                virtual ~PureReduction() { }

                virtual std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<std::vector<const c4::model::expression::IExpression>> consumedExpressions
                ) const = 0;

            protected:
                PureReduction(
                    std::uint32_t stateConsumption,
                    std::uint32_t expressionConsumption
                );
            };
        }
    }
}
