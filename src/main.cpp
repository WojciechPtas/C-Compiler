#include <iostream>
#include "service/io/ISO88591InputStream.h"
#include "lexer.h"
#include "util/token/PrintVisitor.h"

using namespace std;
using namespace c4::service;

int main() {
    shared_ptr<io::ISO88591InputStream> src = std::make_shared<io::ISO88591InputStream>("input.txt");
    // shared_ptr<automata::IAutomaton<char, Token>> keywords = make_shared<automata::IAutomaton<char, Token>>();
    // shared_ptr<automata::IAutomaton<char, Token>> punctuators = make_shared<automata::IAutomaton<char, Token>>();
    c4::Lexer l(src, nullptr, nullptr);
    shared_ptr<Token> token;
    c4::util::token::PrintVisitor pt(cout);


    int i=0;
    while(l.nextToken(token)) {
        cout << ++i << '\n';
    }

    return 0;
}
