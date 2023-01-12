#include <cstdio>
#include <iostream>

#include "model/token/Token.h"
#include "model/token/ErrorToken.h"
#include "service/automata/NodeAutomaton.h"
#include "service/io/FileInputStream.h"
#include "service/io/LexingInputStream.h"
#include "service/io/MetricInputStream.h"
#include "service/io/MosaicInputStream.h"
#include "util/node/NodeUtilities.h"
#include "util/token/KeywordUtilities.h"
#include "util/token/PrintVisitor.h"
#include "util/token/PunctuatorUtilities.h"

#include "service/LLparser/LLParser.h"

using namespace c4::service::parser;
using namespace c4::model::token;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace c4::util::token;
using namespace std;

enum RetCode {
    OK=0, ERR=1
};

int main(int argc, char* argv[]) {
    cout << "dupa\n";
    RetCode retval=OK;
    string input="input.txt";

    for(int i=0; i<argc;i++){
        string in=argv[i];
        if(in == "--tokenize" && i<argc-1){
            input = argv[i+1];
        }
        else if(in =="--parse" && i < argc-1){
            input= argv[i+1];
            //return 0;
        }
    }
    //cout<<input<<endl;

    std::string word;

    auto fileSrc = make_shared<FileInputStream>(input);
    cout << "File stream\n";

    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);
    cout << "mosaic stream\n";

    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);
    cout << "metric\n";

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);
    cout << "automaton\n";

    auto lexer =make_shared<LexingInputStream>(metricSrc, keywordAutomata, punctuatorAutomata);
    cout << "lexer\n";

    shared_ptr<Token> token;
    
    PrintVisitor pt(cout);
    PrintVisitor pe(cerr);

    auto mosaic = make_shared<MosaicInputStream<shared_ptr<Token>>>(lexer,1024);
    cout << "mosaic\n";
    
    LLParser parser(mosaic);
    cout << "parser\n";

    auto a =parser.parse();
    cout<< "PARSED!\n";
    // We adjust the buffering behavior of the standard output stream to
    // increase the efficiency of writing to stdout:
    //
    //  - The default strategy flushes the buffer after each line, which
    //    requires a system call and hence comes with heavy cost.
    //  - We switch from line buffering to full buffering and we set the size
    //    to 4 KiB, which is equivalent to a whole memory page on an x86
    //    system.

    //setvbuf(stdout, nullptr, _IOFBF, 4096);
//
    //while(lexer->read(&token) && !token->isError()) {
    //    token->accept(pt);
    //}
//
    //setvbuf(stdout, nullptr, _IOLBF, 4096);
//
    //if(token->isError()){
    //    token->accept(pe);
    //    retval = ERR;
    //}

    return a;
}
