#include <iostream>
#include "service/io/ISO88591InputStream.h"

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

    return 0;
}
