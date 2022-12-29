#pragma once

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class IInputStream {
            public:
                virtual ~IInputStream() { }
                virtual bool read(TElement *dst) = 0;
            };
        }
    }
}
