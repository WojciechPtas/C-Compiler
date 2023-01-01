#include <cstdio>
#include <memory>

#include "model/token/Token.h"
#include "model/token/ConstantToken.h"
#include "model/token/PunctuatorToken.h"
#include "service/io/VectorInputStream.h"
#include "service/parser/ExpressionParser.h"
#include "util/parser/lr/StateUtilities.h"

using namespace c4::model;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::parser;
using namespace c4::util::parser::lr;
using namespace std;

int main(int argc, char* argv[]) {
    VectorInputStream<shared_ptr<const Token>> input({
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
        )
    });

    auto ptr = make_shared<const State>(INITIAL_STATE); 
    
    ExpressionParser parser(ptr);
    parser.parse(input);
}
