#include <iostream>

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
    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 64);
    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);

    LexingInputStream lexer(metricSrc, keywordAutomata, punctuatorAutomata);

    unique_ptr<Token> token;
    
    PrintVisitor pt(cout);
    PrintVisitor pe(cerr);

    while(lexer.read(&token)) {
        token->accept(pt);
    }

    if(token != nullptr){
        //token->accept(pe);
        retval = ERR;
    }

    return retval;
}
