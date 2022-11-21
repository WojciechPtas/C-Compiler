#include <iostream>
#include "service/io/ISO88591InputStream.h"
#include "lexer.h"
#include "util/token/PrintVisitor.h"

using namespace std;
using namespace c4::service;
using c4::util::token::PrintVisitor;

int main(int argc, char** argv) {
    string input="";
    for(int i=0; i<argc;i++){
        if(argv[i]=="--tokenize"){
        string input = argv[i+1];
        }
    }
    TokenPosition tp("", 0, 0);
    std::string word;

    shared_ptr<io::ISO88591InputStream> src = std::make_shared<io::ISO88591InputStream>("input.txt");
    // shared_ptr<automata::IAutomaton<char, Token>> keywords = make_shared<automata::IAutomaton<char, Token>>();
    // shared_ptr<automata::IAutomaton<char, Token>> punctuators = make_shared<automata::IAutomaton<char, Token>>();
    c4::Lexer l(src, nullptr, nullptr);
    shared_ptr<Token> token;
    int i=0;
    char c;
    // for(i=0; i<20; i++) {
    //     std::cout << c << "\n";
    //     src->read(&c);
    // }
    PrintVisitor pt(cout);
    while(l.nextToken(token)) {
        i++;
        token->accept(pt);
    }
    if(token != nullptr){
        token->accept(pt);
    }
    cout << i << "\n";
    // char c;
    // src->read(&c); std::cout << c;
    // src->read(&c);std::cout << c;
    // src->pushMark();
    // src->read(&c);std::cout << c;
    // src->read(&c);std::cout << c;
    // src->resetToMark();
    // src->read(&c);std::cout << c;
    // src->read(&c);std::cout << c;
    // src->popMark();
    // src->read(&c);std::cout << c;
    // src->read(&c);std::cout << c;

    return 0;
}
