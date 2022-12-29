#pragma once

#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class IBufferedInputStream : public IInputStream<TElement> {
            public:
                virtual ~IBufferedInputStream() { }

                virtual void popMark() = 0;
                virtual void pushMark() = 0;
                virtual void resetToMark() = 0;
                virtual bool lookahead1(TElement *dst) = 0;
            };
        }
    }
}
