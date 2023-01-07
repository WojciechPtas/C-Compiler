#pragma once

#include <memory>
#include <queue>
#include <service/io/IInputStream.h>

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class QueueInputStream : public IInputStream<TElement> {
            public:
                QueueInputStream(
                    std::unique_ptr<std::queue<TElement>> content
                ) : content(std::move(content)) { }

                bool read(TElement *dst) override {
                    if (this->content->empty()) {
                        return false;
                    }

                    *dst = this->content->front();
                    this->content->pop();
                    return true;
                }

            private:
                std::unique_ptr<std::queue<TElement>> content;
            };
        }
    }
}
