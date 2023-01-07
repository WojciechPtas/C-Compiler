#pragma once

#include <cstdlib>
#include <deque>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../../debug.h"
#include "IBufferedInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            template <class TElement>
            class MosaicInputStream : public IBufferedInputStream<TElement> {
                // Design Overview
                // ---------------
                //
                // Invariants:
                //
                //  * Each fragment has exactly fragmentCapacity slots.
                //  * cursorLimit >= cursorOffset
                //  * All marks are greater or equal to zero, newer (more to
                //    the top of the stack) marks are always numerically
                //    greater than the older ones.
                //  * There is always one fragment allocated.

            public:
                MosaicInputStream(
                    const std::shared_ptr<IInputStream<TElement>> &src,
                    std::size_t fragmentCapacity
                ) : fragmentCapacity(fragmentCapacity),
                    source(src),
                    cursorLimit(0),
                    cursorOffset(0),
                    eofReached(false) {
                    // A fragmentCapacity of zero or bellow is invalid, as we
                    // cannot store anything in a fragment that consists of
                    // zero slots.
                    // Since size_t is already an unsigned type, we only have
                    // to check for the zero case.

                    if (fragmentCapacity == 0) {
                        throw std::invalid_argument("fragmentCapacity");
                    }

                    // Allocate the first fragment.

                    this->fragments.push_back(
                        new TElement[this->fragmentCapacity]
                    );

                    DBGOUT_E(
                        "stream",
                        "I: %p, Allocated fragment %p (construction)",
                        this,
                        this->fragments.back()
                    );
                }

                ~MosaicInputStream() {
                    // Deallocate all existing fragments.

                    for (
                        auto iterator = this->fragments.begin();
                        iterator < this->fragments.end();
                        iterator++
                    ) {
                        DBGOUT_E(
                            "stream",
                            "I: %p, Deallocating fragment %p (destruction)",
                            this,
                            *iterator
                        );

                        delete[] *iterator;
                    }
                }

                bool popMark() override {
                    size_t markCount = this->marks.size();

                    if (markCount == 0) {
                        return false;
                    }

                    this->marks.pop_back();

                    if (markCount > 1) {
                        return true;
                    }
                    
                    // Invariant: There is no mark left.
                    //
                    // Hence, we need to deallocate any fragments that preceed
                    // the fragment of cursorOffset, because they cannot be
                    // reached anymore.
                    //
                    // Exception:   If the cursor is at a fragment boundary at
                    //              the moment, i.e. it points to the first
                    //              element of a new fragment, and it is also
                    //              equal to the current limit, this means that
                    //              the next read call will allocate or reuse a
                    //              fragment.
                    //              Since we remove all pevious fragments and
                    //              we must ensure the invariant that there is
                    //              always one fragment allocated, we have to
                    //              preserve the last allocated fragment.
                    //              (Although the cursor does not point to it.)

                    auto shrinkMore =
                        this->cursorOffset >= this->fragmentCapacity;

                    while (shrinkMore) {
                        this->cursorLimit -= this->fragmentCapacity;
                        this->cursorOffset -= this->fragmentCapacity;

                        DBGOUT_E(
                            "stream",
                            "I: %p, Decreased offset to %zu, decreased limit "
                            "to %zu (mark pop)",
                            this,
                            this->cursorOffset,
                            this->cursorLimit
                        );

                        shrinkMore =
                            this->cursorOffset >= this->fragmentCapacity;

                        if (shrinkMore) {
                            DBGOUT_E(
                                "stream",
                                "I: %p, Deallocating fragment %p (mark pop)",
                                this,
                                this->fragments.front()
                            );

                            delete[] this->fragments.front();
                            this->fragments.pop_front();
                        }
                    }

                    return true;
                }

                void pushMark() override {
                    this->marks.push_back(this->cursorOffset);
                }

                bool read(TElement *dst) override {
                    // Idea
                    // ----
                    //
                    // Due to efficiency, reading works in chunks: We always
                    // try to read one whole fragment at once from the
                    // underlying stream.
                    // Afterwards, we serve consecutive reading calls from this
                    // fragment until we either reach its end and do the same
                    // reading again.
                    //
                    // Design
                    // ------
                    //
                    //  * The cursorLimit represents the available number of
                    //    bytes over all fragments. This is either
                    //    fragmentCapacity * fragments.count(), or slightly
                    //    less, if we reached the end of the underlying stream.
                    //
                    //  * The cursorOffset points to the next element that will
                    //    be read. If cursorOffset == cursorLimit, we have to
                    //    refill the buffer first or reached the end.
                    //
                    //  * If a read operation causes the cursorOffset to cross
                    //    fragment boundaries and there is no mark that would
                    //    require us to preserve the previous data in memory,
                    //    we have to deallocate and remove the fragment that we
                    //    just left.
                    //    Due to efficiency there is one exception: If the
                    //    fragment that we left is the only allocated fragment,
                    //    we preserve it to avoid potential costs resulting
                    //    from heap reallocation.

                    if (this->cursorOffset == this->cursorLimit) {
                        // If we reached the eof, we cannot serve any read
                        // request.

                        if (this->eofReached) {
                            return false;
                        }

                        // Otherwise, we try to read a whole fragment.
                        //
                        // If cursorLimit is zero, we (re-)use the last
                        // fragment, otherwise we allocate a new one.

                        TElement *fragment;
                        bool newFragment;

                        if (this->cursorLimit == 0) {
                            fragment = this->fragments.back();
                            newFragment = false;
                        } else {
                            fragment = new TElement[this->fragmentCapacity];
                            newFragment = true;

                            DBGOUT_E(
                                "stream",
                                "I: %p, Allocated fragment %p (read)",
                                this,
                                fragment
                            );
                        }

                        auto readElements = this->source->read(
                            fragment,
                            this->fragmentCapacity
                        );

                        // If we read less elements than fragmentCapacity, we
                        // reached the end of the underlying stream.

                        this->eofReached =
                            readElements < this->fragmentCapacity;

                        // If we read nothing, we cannot serve the read
                        // request.

                        if (readElements == 0) {
                            if (newFragment) {
                                DBGOUT_E(
                                    "stream",
                                    "I: %p, Deallocating fragment %p (read "
                                    "eof)",
                                    this,
                                    fragment
                                );

                                delete[] fragment;
                            }

                            return false;
                        }

                        // If we allocated a new fragment for the data we just
                        // read, we have to add it to the internal stack.

                        if (newFragment) {
                            this->fragments.push_back(fragment);
                        }

                        this->cursorLimit += readElements;

                        DBGOUT_E(
                            "stream",
                            "I: %p, Increased limit to %zu (read)",
                            this,
                            this->cursorLimit
                        );
                    }

                    // Invariant:   At this point, it applies that
                    //              cursorOffset < cursorLimit.

                    *dst = this->fragments
                        [this->cursorOffset / this->fragmentCapacity]
                        [this->cursorOffset % this->fragmentCapacity];

                    this->cursorOffset++;

                    DBGOUT_E(
                        "stream",
                        "I: %p, Increased offset to %zu (read)",
                        this,
                        this->cursorOffset
                    );

                    if (
                        this->cursorOffset % this->fragmentCapacity == 0 &&
                        this->marks.empty()
                    ) {
                        if (this->fragments.size() > 1) {
                            DBGOUT_E(
                                "stream",
                                "I: %p, Deallocating fragment %p (read across "
                                "fragment boundary)",
                                this,
                                this->fragments.front()
                            );

                            delete[] this->fragments.front();
                            this->fragments.pop_front();
                        }

                        this->cursorLimit -= this->fragmentCapacity;
                        this->cursorOffset -= this->fragmentCapacity;

                        DBGOUT_E(
                            "stream",
                            "I: %p, Decreased offset to %zu, decreased limit "
                            "to %zu (read across fragment boundary)",
                            this,
                            this->cursorOffset,
                            this->cursorLimit
                        );
                    }

                    return true;
                }

                bool resetToMark() override {
                    if (this->marks.empty()) {
                        return false;
                    }
                    
                    this->cursorOffset = this->marks.back();

                    DBGOUT_E(
                        "stream",
                        "I: %p, Set offset to %zu (reset)",
                        this,
                        this->cursorOffset
                    );

                    return true;
                }

            private:
                const std::size_t fragmentCapacity;
                const std::shared_ptr<IInputStream<TElement>> source;

                size_t cursorLimit;
                size_t cursorOffset;
                bool eofReached;
                std::deque<TElement*> fragments;
                std::vector<size_t> marks;
            };
        }
    }
}
