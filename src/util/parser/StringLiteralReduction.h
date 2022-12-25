#pragma once

#include "../../model/parser/TokenReduction.h"
#include "../../model/token/StringLiteralToken.h"

namespace c4 {
    namespace util {
        namespace parser {
            class StringLiteralReduction :
                public model::parser::TokenReduction<model::token::StringLiteralToken> {
            public:
                StringLiteralReduction();
                virtual ~StringLiteralReduction() { }

                std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<const model::token::StringLiteralToken> token
                ) const override;
            };
        }
    }
}
