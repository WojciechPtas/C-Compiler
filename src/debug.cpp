#include "debug.h"

#include <cstdarg>
#include <cstdio>

#ifdef DEBUG
using namespace c4;
#endif

using namespace std;

#ifdef DEBUG

void c4::dbgout(string category, string file, string message, ...) {
    va_list args;

    if (string(DEBUG) != "all" && DEBUG != category) {
        return;
    }

    fprintf(stderr, "[DBG %s]: ", file.c_str());

    va_start(args, message);
    vfprintf(stderr, message.c_str(), args);
    va_end(args);

    fprintf(stderr, "\n");
}
#endif
