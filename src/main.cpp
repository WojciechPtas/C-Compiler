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
#include "util/ASTVisitors/PrettyPrintingVisitor/PrettyPrintingVisitor.h"
#include "service/LLparser/LLParser.h"

#include "service/CodeGenerator/codegen.h"

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



std::shared_ptr<c4::model::declaration::IDeclaration> genAST(const std::string& input) {
    auto lexer = initializeLexer(input);
    auto mosaic = make_shared<MosaicInputStream<shared_ptr<Token>>>(lexer,1024);
    LLParser parser(mosaic);
    return parser.parseAndGetAST();
}

enum class ParseCodeGenMode {
    PARSE,
    PRINT,
    CODEGEN,
    DEBUG_CODEGEN
};
bool parseAndCodeGen(const std::string& input, ParseCodeGenMode mode) {
    auto AST = genAST(input);
    bool failed = AST == nullptr;
    if(!failed) {
        CodeGen g(input);
        AST->accept(g);
        if(g.isError()) {
            failed = true;
        }
        else {
            if(mode == ParseCodeGenMode::PRINT) {
                c4::util::pretty::PrettyPrintinVisitor ppv(std::cout);
                AST->accept(ppv);
            }
            if(mode == ParseCodeGenMode::CODEGEN || mode == ParseCodeGenMode::DEBUG_CODEGEN) {
                g.printIR(mode == ParseCodeGenMode::DEBUG_CODEGEN);
            }
        }
    }
    return failed;
}


bool LRparse(const std::string& input) {
    auto lexer = initializeLexer(input);
    auto a=make_shared<State>(INITIAL_STATE);

    auto mosaic = make_shared<MosaicInputStream<shared_ptr<Token>>>(lexer,1024);
    LLParser parser(mosaic);
    
    auto lrparser = make_shared<ExpressionParser>(a, &parser);
    
    auto expr = lrparser->parse(*mosaic);
    
    if(expr == nullptr) {
        c4::util::token::PrintVisitor pt(std::cerr);
        lrparser->lastTokenRead()->accept(pt);
        return ERR;
    }

    c4::util::expression::PrintVisitor pe(std::cout);
    expr->accept(pe);
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
            return parseAndCodeGen(input, ParseCodeGenMode::PARSE);
        }
        else if(in=="--print-ast"){
            input= argv[i+1];
            return parseAndCodeGen(input, ParseCodeGenMode::PRINT);
        }
        else if(in =="--lrparse" && i < argc-1){
            input= argv[i+1];
            return LRparse(input);
        }
        else if(in =="--compile") {
            input= argv[i+1];
            return parseAndCodeGen(input, ParseCodeGenMode::CODEGEN);
        }
        else if(in =="--dcompile") {
            input= argv[i+1];
            return parseAndCodeGen(input, ParseCodeGenMode::DEBUG_CODEGEN);
        }
    }
    cerr << "No command given\n";
    return 1;
}
