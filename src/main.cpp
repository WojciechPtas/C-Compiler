#include <iostream>

#include "lexer.h"
#include "service/automata/NodeAutomaton.h"
#include "service/io/ISO88591InputStream.h"
#include "util/node/NodeUtilities.h"
#include "util/token/KeywordUtilities.h"
#include "util/token/PrintVisitor.h"
#include "util/token/PunctuatorUtilities.h"

using namespace c4::model::token;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace c4::util::token;
using namespace std;

int main(int argc, char* argv[]) {
    string input="input.txt";
    for(int i=0; i<argc;i++){
        string in=argv[i];
        if(in == "--tokenize" && i<argc-1){
            input = argv[i+1];
        }
    }
    //cout<<input<<endl;
    TokenPosition tp("", 0, 0);
    std::string word;

    shared_ptr<ISO88591InputStream> src = std::make_shared<ISO88591InputStream>(input);
    c4::Lexer l(
        src,
        make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE),
        make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE)
    );
    shared_ptr<const Token> token;
    int i=0;
    
    PrintVisitor pt(cout);
    while(l.nextToken(token)) {
        i++;
        token->accept(pt);
    }
    if(token != nullptr){
        token->accept(pt);
    }
    cout << i << "\n";

    return 0;
}
