#include "ErrorToken.h"

using namespace c4::model::token;
using namespace c4::util::token;
using namespace std;

ErrorToken::ErrorToken(TokenPosition position, string message)
    : Token(position), message(message) {
}

void ErrorToken::accept(ITokenVisitor &visitor) {
    visitor.visit(*this);
}

const std::string &ErrorToken::getMessage() {
    return this->message;
}
