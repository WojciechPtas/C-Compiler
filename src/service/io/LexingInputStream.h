#pragma once

#include <memory>

#include "../../model/token/Token.h"
#include "../../model/token/Keyword.h"
#include "../../model/token/Punctuator.h"
#include "../automata/IAutomaton.h"
#include "IInputStream.h"
#include "MetricInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            /**
             * An input stream that reads characters from an underlying stream
             * and transforms them to tokens.
             */
            class LexingInputStream :
                public IInputStream<std::unique_ptr<model::token::Token>> {
            public:
                LexingInputStream(
                    const std::shared_ptr<MetricInputStream> &src,
                    const std::shared_ptr<const automata::IAutomaton<char, model::token::Keyword>> &keywordAutomaton,
                    const std::shared_ptr<const automata::IAutomaton<char, model::token::Punctuator>> &punctuatorAutomaton
                );

                bool read(
                    std::unique_ptr<model::token::Token> *dst
                ) override;

            private:
                const std::shared_ptr<const automata::IAutomaton<char, model::token::Keyword>> keywordAutomaton;
                const std::shared_ptr<const automata::IAutomaton<char, model::token::Punctuator>> punctuatorAutomaton;
                const std::shared_ptr<MetricInputStream> source;

                bool eofReached;
                bool isEmpty;

                void readCharacterConstant(
                    std::unique_ptr<model::token::Token> *dst
                );

                void readDecimalConstant(
                    std::unique_ptr<model::token::Token> *dst
                );

                void readIdentifierOrKeyword(
                    std::unique_ptr<model::token::Token> *dst
                );

                void readPunctuatorOrError(
                    std::unique_ptr<model::token::Token> *dst
                );

                void readStringLiteral(
                    std::unique_ptr<model::token::Token> *dst
                );
            };
        }
    }
}
