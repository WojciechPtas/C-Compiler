#pragma once

#include <memory>

#include "../expression/IExpression.h"
#include "Reduction.h"

namespace c4 {
    namespace model {
        namespace parser {
            template <class TToken>
            class TokenReduction : public Reduction {
            public:
                virtual ~TokenReduction() { }

                virtual std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<const TToken> token
                ) const = 0;

            protected:
                TokenReduction() : Reduction(1, 0) { }
            };
        }
    }
}
