#pragma once

#include "DelimiterInputStream.h"

namespace c4::service::io{
        class ParenthesisDelimiterStream: public DelimiterStream{
            int counter;
            public:
            ParenthesisDelimiterStream(
                    std::shared_ptr<IBufferedInputStream<std::shared_ptr<model::token::Token>>> src//,
                    //util::token::TokenKind kind,
                    //util::token::SpecifiedToken specific
                ) : DelimiterStream(src,util::token::TokenKind::identifier,util::token::SpecifiedToken()), counter(1) {};
            bool read(std::shared_ptr<model::token::Token> *dst) override
            {
                    src->pushMark();
                    bool a = src->read(dst);
                    if(!a) return false;
                    (*dst)->accept(visitor);
                    if(visitor.getKind()!=util::token::TokenKind::punctuator){
                        src->popMark();
                        return true;
                    }
                    else
                    {
                        if(visitor.getSepcificValue().p==c4::model::token::Punctuator::LeftParenthesis) ++counter;
                        else if(visitor.getSepcificValue().p==c4::model::token::Punctuator::RightParenthesis) --counter;
                    }
                    if(counter)
                    {
                        src->popMark();
                        return true;
                    }
                    else{
                        src->resetAndPopMark();
                        return false;
                    }
                        

}
            };
        };
    