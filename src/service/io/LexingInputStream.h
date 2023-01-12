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
                public IInputStream<std::shared_ptr<model::token::Token>> {
            public:
                LexingInputStream(
                    const std::shared_ptr<MetricInputStream> &src,
                    const std::shared_ptr<const automata::IAutomaton<char, model::token::Keyword>> &keywordAutomaton,
                    const std::shared_ptr<const automata::IAutomaton<char, model::token::Punctuator>> &punctuatorAutomaton
                );

                bool read(
                    std::shared_ptr<model::token::Token> *dst
                ) override;

            private:
                const std::shared_ptr<const automata::IAutomaton<char, model::token::Keyword>> keywordAutomaton;
                const std::shared_ptr<const automata::IAutomaton<char, model::token::Punctuator>> punctuatorAutomaton;
                const std::shared_ptr<MetricInputStream> source;

                bool eofReached;
                bool isEmpty;

                void readCharacterConstant(
                    std::shared_ptr<model::token::Token> *dst
                );

                void readDecimalConstant(
                    std::shared_ptr<model::token::Token> *dst
                );

                void readIdentifierOrKeyword(
                    std::shared_ptr<model::token::Token> *dst
                );

                void readPunctuatorOrError(
                    std::shared_ptr<model::token::Token> *dst
                );

                void readStringLiteral(
                    std::shared_ptr<model::token::Token> *dst
                );
            };
        }
    }
}
