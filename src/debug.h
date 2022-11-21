#pragma once

#ifdef DEBUG
#include <string>

namespace c4 {
    void dbgout(
        std::string category,
        std::string file,
        std::string message,
        ...
    );
}

#define DBGOUT(category, message) c4::dbgout(category, __FILE__, message)
#define DBGOUT_E(category, message, ...) \
    c4::dbgout(category, __FILE__, message, __VA_ARGS__)
#else
#define DBGOUT(category, message) ;
#define DBGOUT_E(category, message, ...) ;
#endif
