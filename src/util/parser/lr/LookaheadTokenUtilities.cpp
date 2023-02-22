#include "LookaheadTokenUtilities.h"

#include "../../token/ParserVisitor.h"
#include "../../../model/declaration/TypeName.h"
#include "../../../model/expression/SizeOfType.h"
#include "../../../model/token/Punctuator.h"
#include "../../../model/token/Keyword.h"

namespace c4::util::parser {

bool isSizeOf(std::shared_ptr<const c4::model::token::Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::keyword &&
        pv.getSepcificValue().k == c4::model::token::Keyword::Sizeof
    );
}

bool isTypeSpecifier(std::shared_ptr<const c4::model::token::Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::keyword && (
            pv.getSepcificValue().k == c4::model::token::Keyword::Char ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Double ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Float ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Int ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Long ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Short ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Signed ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Struct ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Unsigned ||
            pv.getSepcificValue().k == c4::model::token::Keyword::Void ||
            pv.getSepcificValue().k == c4::model::token::Keyword::__Bool
        )
    );
}

bool isLeftParenthesis(std::shared_ptr<const c4::model::token::Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::punctuator && (
            pv.getSepcificValue().p == c4::model::token::Punctuator::LeftParenthesis
        )
    );
}

bool isRightParenthesis(std::shared_ptr<const c4::model::token::Token> token) {
    c4::util::token::ParserVisitor pv;
    token->accept(pv);
    return (
        pv.getKind() == c4::util::token::TokenKind::punctuator && (
            pv.getSepcificValue().p == c4::model::token::Punctuator::RightParenthesis
        )
    );
}
}