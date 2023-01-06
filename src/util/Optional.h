#pragma once

namespace c4 {
    namespace util {
        template <class TValue>
        struct Optional {
            bool isEmpty;
            TValue value;

            Optional() : isEmpty(true) { }

            Optional &operator=(const TValue &value) {
                this->isEmpty = false;
                this->value = value;
                return *this;
            }
        };
    }
}
