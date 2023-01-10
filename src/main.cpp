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

using namespace c4::model::token;
using namespace c4::service::automata;
using namespace c4::service::io;
using namespace c4::util::token;
using namespace std;

enum RetCode {
    OK=0, ERR=1
};

int main(int argc, char* argv[]) {
    RetCode retval=OK;
    string input="input.txt";
    for(int i=0; i<argc;i++){
        string in=argv[i];
        if(in == "--tokenize" && i<argc-1){
            input = argv[i+1];
        }
        else if(in =="--parse" && i < argc-1){
            input= argv[i+1];
            return 0;
        }
    }
    //cout<<input<<endl;
    std::string word;

    auto fileSrc = make_shared<FileInputStream>(input);
    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);
    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);

    LexingInputStream lexer(metricSrc, keywordAutomata, punctuatorAutomata);

    unique_ptr<Token> token;
    
    PrintVisitor pt(cout);

    // We adjust the buffering behavior of the standard output stream to
    // increase the efficiency of writing to stdout:
    //
    //  - The default strategy flushes the buffer after each line, which
    //    requires a system call and hence comes with heavy cost.
    //  - We switch from line buffering to full buffering and we set the size
    //    to 4 KiB, which is equivalent to a whole memory page on an x86
    //    system.

    setvbuf(stdout, nullptr, _IOFBF, 4096);

    while(lexer.read(&token)) {
        token->accept(pt);
    }

    setvbuf(stdout, nullptr, _IOLBF, 4096);

    if(dynamic_cast<ErrorToken*>(token.get())){
        retval = ERR;
    }

    return retval;
}
