#pragma once

#include "../../model/parser/TokenReduction.h"
#include "../../model/token/IdentifierToken.h"

namespace c4 {
    namespace util {
        namespace parser {
            class IdentifierReduction :
                public model::parser::TokenReduction<model::token::IdentifierToken> {
            public:
                IdentifierReduction();
                virtual ~IdentifierReduction() { }

                std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<const model::token::IdentifierToken> token
                ) const override;
            };
        }
    }
}
