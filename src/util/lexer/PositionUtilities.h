#pragma once

#include "../../model/token/TokenPosition.h"
#include "../../service/io/MetricInputStream.h"

namespace c4 {
    namespace util {
        namespace lexer {
            #define MAKE_TOKEN_POSITION(varName, stream) \
                TokenPosition varName( \
                    (stream).filePath, \
                    (stream).getLine(), \
                    (stream).getColumn() \
                )
        }
    }
}
