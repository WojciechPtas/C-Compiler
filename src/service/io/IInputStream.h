#pragma once

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class IInputStream {
            public:
                virtual ~IInputStream() { }

                virtual void popMark() = 0;
                virtual void pushMark() = 0;
                virtual bool read(TElement *dst) = 0;
                virtual void resetToMark() = 0;
            };
        }
    }
}
