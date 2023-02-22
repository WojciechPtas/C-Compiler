#include <memory>
#include "../../../model/token/Token.h"


namespace c4::util::parser {

bool isSizeOf(std::shared_ptr<const c4::model::token::Token> token);

bool isTypeSpecifier(std::shared_ptr<const c4::model::token::Token> token);

bool isLeftParenthesis(std::shared_ptr<const c4::model::token::Token> token);

bool isRightParenthesis(std::shared_ptr<const c4::model::token::Token> token);

}