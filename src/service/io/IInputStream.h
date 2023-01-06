#pragma once

#include <cstddef>

namespace c4 {
    namespace service {
        namespace io {
            /**
             * A readonly stream of elements.
             *
             * @param TElement  The common type of the element instances that
             *                  can be read from this stream.
             */
            template <class TElement>
            class IInputStream {
            public:
                virtual ~IInputStream() { }

                /**
                 * Attempts to read an element from this stream into the
                 * specified destination.
                 *
                 * @param dst   The destination of the read element. If no
                 *              element has been read, e.g. due to
                 *              end-of-stream, the value pointed to by the
                 *              specified pointer is implementation-specific.
                 *
                 * @return  Either true, if the read operation succeeds and an
                 *          element has been stored at dst, or otherwise false,
                 *          if either an error occurred or the end of the
                 *          stream has been reached.
                 */
                virtual bool read(TElement *dst) = 0;

                /**
                 * Reads up to maxCount elements from this stream into the
                 * specified destination.
                 *
                 * @param dst       The destination of the read elements. This
                 *                  pointer will be treated as an array of at
                 *                  least maxCount TElement instances.
                 *                  Note that if n < maxCount, where n is the
                 *                  return value of this function, only the
                 *                  first n elements of dst are valid, while
                 *                  the values of the remaining maxCount - n
                 *                  elements is implementation-specific.
                 *
                 * @param maxCount  The maximum number of elements that will be
                 *                  read and stored at dst.
                 *
                 * @return  The actual number of elements that have been read
                 *          and stored at dst. It always applies that this is
                 *          less or equal to maxCount.
                 */
                virtual std::size_t read(
                    TElement *dst,
                    std::size_t maxCount
                ) {
                    // NOTE:    Only a very naive implementation, just in case
                    //          there is no better option available.

                    std::size_t i;

                    for (i = 0; i < maxCount; i++) {
                        if (!this->read(&dst[i])) {
                            break;
                        }
                    }

                    return i;
                }
            };
        }
    }
}
