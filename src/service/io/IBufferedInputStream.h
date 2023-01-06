#pragma once

#include <cstddef>
#include "IInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            /**
             * An special IInputStream that provides support for rewinding the
             * data stream to a previously marked point.
             */
            template <class TElement>
            class IBufferedInputStream : public IInputStream<TElement> {
            public:
                virtual ~IBufferedInputStream() { }

                /**
                 * Drops all marks that have been pushed, but not removed yet.
                 *
                 * @return  The number of marks that have been found and
                 *          removed.
                 */
                virtual std::size_t clearMarks() {
                    // NOTE:    Naive implementation, if there is no better
                    //          strategy or the implementation author is lazy.

                    size_t count;
                    for (count = 0; this->popMark(); count++) ;
                    return count;
                }

                /**
                 * Moves the last mark, that has been placed, to the current
                 * read position.
                 *
                 * If there is no mark available, this method returns false and
                 * has no further effect, i.e. it does not place a mark.
                 *
                 * @return  Either true, if the move was successful, otherwise,
                 *          if there was no mark, false.
                 */
                virtual bool moveMark() {
                    // NOTE:    Naive implementation, if there is no better
                    //          strategy or the implementation author is lazy.

                    if (!this->popMark()) {
                        return false;
                    }

                    this->pushMark();
                    return true;
                }

                /**
                 * Removes the last mark that has been placed.
                 *
                 * @return  Whether there was a mark that has been popped
                 *          (true), or not (false).
                 */
                virtual bool popMark() = 0;

                /**
                 * Attempts to read the next element of the stream without
                 * moving the cursor, i.e. if there is a next element, the next
                 * read or peek operation on this stream, will return the same
                 * element.
                 *
                 * @param dst   A pointer to the memory where the read element
                 *              will be stored at. If there is no next element,
                 *              the stored value will is
                 *              implementation-specific.
                 *
                 * @return  Either true, if there is a next element and it has
                 *          been stored at dst, or otherwise false.
                 */
                virtual bool peek(TElement *dst) {
                    // NOTE:    Naive implementation, if there is no better
                    //          strategy or the implementation author is lazy.

                    this->pushMark();
                    auto exists = this->read(dst);
                    this->resetAndPopMark();

                    return exists;
                }

                /**
                 * Places a mark at the current stream position.
                 */
                virtual void pushMark() = 0;

                /**
                 * Resets this stream to the latest mark that was pushed, but
                 * not yet popped, and pops this very mark.
                 *
                 * If there is no such mark available, this operation has no
                 * effect.
                 *
                 * @return  Either true, if there was a mark that the stream
                 *          has been reset to and the mark has been popped, or
                 *          otherwise false.
                 */
                virtual bool resetAndPopMark() {
                    // NOTE:    Naive implementation, if there is no better
                    //          strategy or the implementation author is lazy.

                    return this->resetToMark() && this->popMark();
                }

                /**
                 * Resets this stream to the latest mark that was pushed, but
                 * not yet popped.
                 *
                 * @return  Whether there was a mark that the stream has been
                 *          reset to (true), or not (false).
                 */
                virtual bool resetToMark() = 0;

                //virtual bool lookahead1(TElement *dst) = 0;
            };
        }
    }
}
