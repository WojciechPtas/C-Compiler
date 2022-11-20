#pragma once
#include "service/io/ISO88591InputStream.h"
#include "model/token/Token.h"

using namespace c4::model::token;
using namespace c4::service::io; //I hope this is fine

namespace c4 {

class Lexer {
private:
    ISO88591InputStream charStream;

public:
    Lexer(const std::string& filePath) : charStream(filePath) {}
    bool nextToken(const Token* token);
};

}