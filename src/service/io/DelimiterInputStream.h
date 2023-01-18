#pragma once
#include "../../debug.h"
#include "../../util/token/TokenKind.h"
#include "../../util/token/ParserVisitor.h"

#include "MosaicInputStream.h"
namespace c4 {
    namespace service {
        namespace io {
            class DelimiterStream : public IBufferedInputStream<std::shared_ptr<model::token::Token>>{
                protected:
                    std::shared_ptr<IBufferedInputStream<std::shared_ptr<model::token::Token>>> src;
                    util::token::TokenKind kind;
                    util::token::SpecifiedToken specific;
                    util::token::ParserVisitor visitor;
                public:
                DelimiterStream(
                    std::shared_ptr<IBufferedInputStream<std::shared_ptr<model::token::Token>>> src,
                    util::token::TokenKind kind,
                    util::token::SpecifiedToken specific
                ) : src(src), kind(kind), specific(specific) {};
                bool read(std::shared_ptr<model::token::Token> *dst) override {
                    src->pushMark();
                    bool a = src->read(dst);
                    if(!a) return false;
                    (*dst)->accept(visitor);
                    if(visitor.getKind()!=kind){
                        src->popMark();
                        return true;
                    }
                    else{
                        switch(visitor.getKind()){
                            case util::token::TokenKind::punctuator:
                            if(visitor.getSepcificValue().p==specific.p){
                            src->resetAndPopMark();
                            return false;
                            }
                            break;
                            case util::token::TokenKind::keyword:
                            if(visitor.getSepcificValue().k==specific.k){
                            src->resetAndPopMark();
                            return false;
                            }
                            break;
                            default:
                            break;
                        }
                            src->popMark();
                            return true;
                        
                    }
                }
                void pushMark()override
                {src->pushMark();
                return;
                }
                bool popMark()override
                {
                   return src->popMark();
                
                }
                bool resetToMark(){
                   return src->resetToMark();                   
                    }
            };
            }
            }
            } 