#pragma once
#include <string>

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

                //TO BE IMPLEMENTED
                virtual uint32_t getPosLine() const {return 0;};
                virtual uint32_t getPosColumn() const {return 0;};
                virtual std::string getSourceName() const {return "ImplementMe.cpp";}
            };
        }
    }
}
