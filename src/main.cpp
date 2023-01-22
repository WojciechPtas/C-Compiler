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
#include "service/io/DelimiterInputStream.h"

#include "util/node/NodeUtilities.h"
#include "util/token/KeywordUtilities.h"
#include "util/token/PunctuatorUtilities.h"
#include "service/parser/ExpressionParser.h"
#include "util/parser/lr/StateUtilities.h"
#include "model/parser/lr/State.h"
#include "service/parser/ExpressionParser.h"
#include "service/LLparser/LLParser.h"
#include "util/parser/lr/StateUtilities.h"
using namespace c4::model;
using namespace c4::model::parser::lr;
using namespace c4::service::parser;
using namespace c4::model::token;
using namespace c4::service::io;
using namespace c4::service::automata;
using namespace c4::service::parser;
//using namespace c4::util::expression;
using namespace c4::util::parser::lr;
using namespace c4::util::token;
using namespace std;
using namespace c4::service::parser;
using namespace c4::model::parser::lr;
enum RetCode {
    OK=0, ERR=1
};

enum Action{
    TOKENIZE,
    PARSE,
    PRETTYPRINT,
    COMPILE
};


bool tokenize(std::string input) {
    ///cout << "dupa\n";
    RetCode retval=OK;
    

    auto fileSrc = make_shared<FileInputStream>(input);
    //cout << "File stream\n";

    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);
    //cout << "mosaic stream\n";

    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);
    //cout << "metric\n";

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);
    //cout << "automaton\n";

    auto lexer =LexingInputStream(metricSrc, keywordAutomata, punctuatorAutomata);
    //cout << "lexer\n";

    shared_ptr<Token> token;
    
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

    if(token->isError()) {
        token->accept(pe);
        retval = ERR;
    }
    return retval;
}

bool parse(std::string input) {
        ///cout << "dupa\n";
    //RetCode retval=OK;
    //string input="input.txt";

    auto fileSrc = make_shared<FileInputStream>(input);
    //cout << "File stream\n";

    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);
    //cout << "mosaic stream\n";

    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);
    //cout << "metric\n";

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);
    //cout << "automaton\n";

    auto lexer =make_shared<LexingInputStream>(metricSrc, keywordAutomata, punctuatorAutomata);
    //cout << "lexer\n";

    shared_ptr<Token> token;
    
    PrintVisitor pt(cout);
    PrintVisitor pe(cerr);

    auto mosaic = make_shared<MosaicInputStream<shared_ptr<Token>>>(lexer,1024);
    //cout << "mosaic\n";
    auto st = make_shared<DelimiterStream>(mosaic, TokenKind::punctuator,SpecifiedToken(Punctuator::Semicolon));
    auto a=make_shared<State>(INITIAL_STATE);
    auto lrparser = make_shared<ExpressionParser>(a);
    LLParser parser(mosaic);
    //cout << "parser\n";

    //auto re =parser.parse();
    //cout<< "Returned with:!" << re << "\n";
    return parser.run();
}

int main(int argc, char* argv[]) {
    string input="input.txt";
    for(int i=0; i<argc;i++){
        string in=argv[i];
        if(in == "--tokenize" && i<argc-1){
            input = argv[i+1];
            return tokenize(input);
        }
        else if(in =="--parse" && i < argc-1){
            input= argv[i+1];
            return parse(input);
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
