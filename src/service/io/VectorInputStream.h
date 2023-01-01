#pragma once

#include <vector>
#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class VectorInputStream : public IInputStream<TElement> {
            public:
                VectorInputStream(
                    std::vector<TElement> elements
                ) : elements(elements) {
                    this->iterator = this->elements.begin();
                }

                bool read(TElement *dst) {
                    if (this->iterator == this->elements.end()) {
                        return false;
                    }

                    *dst = *this->iterator;
                    this->iterator++;
                    return true;
                }

            private:
                std::vector<TElement> elements;
                typename std::vector<TElement>::iterator iterator;
            };
        }
    }
}
