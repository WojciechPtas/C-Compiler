#pragma once

#include "../../model/parser/TokenReduction.h"
#include "../../model/token/DecimalConstantToken.h"

namespace c4 {
    namespace util {
        namespace parser {
            class DecimalConstantReduction :
                public model::parser::TokenReduction<model::token::DecimalConstantToken> {
            public:
                DecimalConstantReduction();
                virtual ~DecimalConstantReduction() { }

                std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<const model::token::DecimalConstantToken> token
                ) const override;
            };
        }
    }
}
