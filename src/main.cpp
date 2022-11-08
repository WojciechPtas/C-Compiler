#include <iostream>

#include "model/token/KeywordToken.h"
#include "util/token/PrintVisitor.h"

using namespace std;

int main(int argc, const char *argv[]) {
    c4::model::token::KeywordToken kwToken(
        c4::model::token::TokenPosition("test.c", 1, 1),
        c4::model::token::Keyword::While
    );

    c4::util::token::PrintVisitor visitor(std::cout);

    kwToken.accept(visitor);

    return 0;
}
