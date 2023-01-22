#include <iostream>
#include <memory>
#include "model/token/Token.h"
#include "util/token/PrintVisitor.h"
#include "service/automata/NodeAutomaton.h"

#include "service/io/FileInputStream.h"
#include "service/io/LexingInputStream.h"
#include "service/io/MetricInputStream.h"
#include "service/io/MosaicInputStream.h"
#include "service/io/DelimiterInputStream.h"

#include "model/parser/lr/State.h"
#include "model/expression/IExpression.h"
#include "util/node/NodeUtilities.h"
#include "util/token/KeywordUtilities.h"
#include "util/token/PunctuatorUtilities.h"
#include "util/parser/lr/StateUtilities.h"
#include "service/parser/ExpressionParser.h"
#include "util/expression/PrintVisitor.h"
#include "service/LLparser/LLParser.h"

using namespace c4::service::parser;
using namespace c4::model::token;
using namespace c4::model::parser::lr;
using namespace c4::util::parser::lr;
using namespace c4::service::io;
using namespace c4::service::automata;
using namespace c4::util::token;
using namespace std;

enum RetCode {
    OK=0, ERR=1
};

enum Action{
    TOKENIZE,
    PARSE,
    PRETTYPRINT,
    COMPILE
};


shared_ptr<LexingInputStream> initializeLexer(const std::string& input) {
     auto fileSrc = make_shared<FileInputStream>(input);

    auto bufferedSrc = make_shared<MosaicInputStream<char>>(fileSrc, 1024);

    auto metricSrc = make_shared<MetricInputStream>(bufferedSrc, input);

    auto keywordAutomata = make_shared<NodeAutomaton<char, Keyword>>(KEYWORD_TREE);
    auto punctuatorAutomata = make_shared<NodeAutomaton<char, Punctuator>>(PUNCTUATOR_TREE);

    return make_shared<LexingInputStream>(metricSrc, keywordAutomata, punctuatorAutomata);
}

bool tokenize(const std::string &input) {
    RetCode retval=OK;

    auto lexer = initializeLexer(input);

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
    
    while(lexer->read(&token) && !token->isError()) {
        token->accept(pt);
    }

    if(token->isError()) {
        token->accept(pe);
        retval = ERR;
    }
    return retval;
}

bool parse(const std::string& input) {
    auto lexer = initializeLexer(input);

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

bool LRparse(const std::string& input) {
    auto lexer = initializeLexer(input);
    auto a=make_shared<State>(INITIAL_STATE);
    auto lrparser = make_shared<ExpressionParser>(a);
    
    auto expr = lrparser->parse(*lexer);
    
    if(expr == nullptr) {
        return ERR;
    }

    c4::util::expression::PrintVisitor pt(std::cout);
    expr->accept(pt);
    return OK;
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
        else if(in =="--lrparse" && i < argc-1){
            input= argv[i+1];
            return LRparse(input);
        }
    }
    cerr << "No command given\n";
}
