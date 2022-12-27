#pragma once

#include "../token/Punctuator.h"
#include "ITokenCondition.h"

namespace c4 {
    namespace model {
        namespace parser {
            class PunctuatorCondition : ITokenCondition {
            public:
                PunctuatorCondition(token::Punctuator punctuator);
                virtual ~PunctuatorCondition() { }

                std::unique_ptr<ITokenConditionVerifier> createVerifier() const override;

            private:
                const token::Punctuator punctuator;
            };
        }
    }
}
