#include <iostream>
#include <memory>

#include "model/token/Token.h"
#include "model/token/ConstantToken.h"
#include "model/token/IdentifierToken.h"
#include "model/token/PunctuatorToken.h"
#include "service/io/VectorInputStream.h"
#include "service/parser/ExpressionParser.h"
#include "util/expression/PrintVisitor.h"
#include "util/parser/lr/StateUtilities.h"

using namespace c4::model;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::parser;
using namespace c4::util::expression;
using namespace c4::util::parser::lr;
using namespace std;

int main(int argc, char* argv[]) {
    VectorInputStream<shared_ptr<const Token>> input({
        make_shared<IdentifierToken>(
            TokenPosition("def.c", 0, 0),
            "a"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Equal
        ),
        make_shared<IdentifierToken>(
            TokenPosition("def.c", 0, 0),
            "b"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Equal
        ),
        make_shared<IdentifierToken>(
            TokenPosition("def.c", 0, 0),
            "function"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::LeftParenthesis
        ),
        make_shared<ConstantToken>(
            TokenPosition("abc.c", 0, 0),
            ConstantType::Decimal,
            "518"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Comma
        ),
        make_shared<ConstantToken>(
            TokenPosition("abc.c", 0, 0),
            ConstantType::Decimal,
            "234"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Plus
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "1337"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Asterisk
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "47"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::LessThan
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::ExclamationMark
        ),
        make_shared<IdentifierToken>(
            TokenPosition("def.c", 0, 0),
            "value"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::QuestionMark
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "666"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Colon
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "42"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::RightParenthesis
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Plus
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "42"
        ),
        make_shared<PunctuatorToken>(
            TokenPosition("def.c", 0, 0),
            Punctuator::Asterisk
        ),
        make_shared<ConstantToken>(
            TokenPosition("ghi.c", 0, 0),
            ConstantType::Decimal,
            "69"
        ),
    });

    auto ptr = make_shared<const State>(INITIAL_STATE); 
    
    ExpressionParser parser(ptr);
    PrintVisitor visitor(cout);
    parser.parse(input)->accept(visitor);
}
