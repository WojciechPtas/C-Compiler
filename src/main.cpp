#include <iostream>
#include "service/io/ISO88591InputStream.h"
#include "lexer.h"

using namespace std;

int main() {
    c4::service::io::ISO88591InputStream src("Makefile");
    char character;

    while (src.read(&character)) {
        src.pushMark();
        cout << character;

        src.resetToMark();
        cout << character;

        src.popMark();
    }

    cout << "\n\n<EOF>\n\n";
    c4::Lexer l("a");

    return 0;
}
