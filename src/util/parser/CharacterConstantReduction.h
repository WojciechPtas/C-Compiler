#pragma once

#include "../../model/parser/TokenReduction.h"
#include "../../model/token/CharacterConstantToken.h"

namespace c4 {
    namespace util {
        namespace parser {
            class CharacterConstantReduction :
                public model::parser::TokenReduction<model::token::CharacterConstantToken> {
            public:
                CharacterConstantReduction();
                virtual ~CharacterConstantReduction() { }

                std::shared_ptr<const c4::model::expression::IExpression> reduce(
                    std::shared_ptr<const model::token::CharacterConstantToken> token
                ) const override;
            };
        }
    }
}
