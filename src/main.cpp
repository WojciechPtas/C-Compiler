#include <cstdio>
#include <iostream>
#include <memory>
#include "model/token/Token.h"
#include "util/token/PrintVisitor.h"
// #include "model/token/ErrorToken.h"
#include "service/automata/NodeAutomaton.h"

#include "service/io/FileInputStream.h"
#include "service/io/LexingInputStream.h"
#include "service/io/MetricInputStream.h"
#include "service/io/MosaicInputStream.h"

#include "util/node/NodeUtilities.h"
#include "util/token/KeywordUtilities.h"
#include "util/token/PunctuatorUtilities.h"
#include "service/parser/ExpressionParser.h"
#include "util/parser/lr/StateUtilities.h"

using namespace c4::model;
using namespace c4::model::parser::lr;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::automata;
using namespace c4::service::parser;
// using namespace c4::util::expression;
using namespace c4::util::parser::lr;
using namespace c4::util::token;
using namespace std;

enum RetCode {
    OK=0, ERR=1
};

string input="input.txt";

bool tokenize() {
    RetCode retval=OK;
    std::string word;

    auto fileSrc = make_shared<FileInputStream>(input);
    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);
    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);

    LexingInputStream lexer(metricSrc, keywordAutomata, punctuatorAutomata);

    unique_ptr<Token> token;
    
    PrintVisitor pt(cout);
    PrintVisitor pe(cerr);

    // We adjust the buffering behavior of the standard output stream to
    // increase the efficiency of writing to stdout:
    //
    //  - The default strategy flushes the buffer after each line, which
    //    requires a system call and hence comes with heavy cost.
    //  - We switch from line buffering to full buffering and we set the size
    //    to 4 KiB, which is equivalent to a whole memory page on an x86
    //    system.

    setvbuf(stdout, nullptr, _IOFBF, 4096);

    
    while(lexer.read(&token) && !token->isError()) {
        token->accept(pt);
    }

    if(token != nullptr && token->isError()) {
        token->accept(pe);
        retval = ERR;
    }

    return retval;
}

bool parse() {
    return 1;
}

int main(int argc, char* argv[]) {
    for(int i=0; i<argc;i++){
        string in=argv[i];
        if(in == "--tokenize" && i<argc-1){
            input = argv[i+1];
            return tokenize();
        }
        else if(in =="--parse" && i < argc-1){
            input= argv[i+1];
            return parse();
        }
    }
    //cout<<input<<endl;
    

    /*
      auto ptr = make_shared<const State>(INITIAL_STATE); 
    
    ExpressionParser parser(ptr);
    PrintVisitor visitor(cout);
    parser.parse(input)->accept(visitor);
    */
}
