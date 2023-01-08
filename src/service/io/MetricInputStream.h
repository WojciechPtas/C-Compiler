#pragma once

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

#include "IBufferedInputStream.h"
#include "../../util/Optional.h"

namespace c4 {
    namespace service {
        namespace io {
            struct MetricSnapshot {
                std::size_t column;
                util::Optional<std::size_t> lastColumn;
                util::Optional<std::size_t> lastLine;
                std::size_t line;
            };

            class MetricInputStream : public IBufferedInputStream<char> {
            public:
                /**
                 * The path of the underlying file as passed at construction.
                 */
                const std::string_view filePath;

                MetricInputStream(
                    const std::shared_ptr<IBufferedInputStream<char>> &src,
                    const std::string_view &filePath
                );

                /**
                 * Returns the one-based column of the next character that will
                 * be read.
                 * 
                 * Note that since we have not read the next character yet, we
                 * do not know if it exists and hence its position may be
                 * invalid.
                 *
                 * @return The column of the next character that will be read.
                 */
                std::size_t getColumn() const;

                /**
                 * Attempts to place the column of the last character, that has
                 * been read successfully, into the memory that dst points to.
                 *
                 * @param dst   The memory where the column of last character
                 *              will be stored at. If there was no such
                 *              character, e.g. because we are at the beginning
                 *              of the stream, the stored value is
                 *              implementation-specific.
                 *
                 * @return  Either true, if there was a last character and its
                 *          column has been stored at dst, or otherwise false.
                 */
                bool getLastColumn(size_t *dst) const;

                /**
                 * Attempts to place the line of the last character, that has
                 * been read successfully, into the memory that dst points to.
                 *
                 * @param dst   The memory where the line of last character
                 *              will be stored at. If there was no such
                 *              character, e.g. because we are at the beginning
                 *              of the stream, the stored value is
                 *              implementation-specific.
                 *
                 * @return  Either true, if there was a last character and its
                 *          line has been stored at dst, or otherwise false.
                 */
                bool getLastLine(size_t *dst) const;

                /**
                 * Returns the one-based line of the next character that will
                 * be read.
                 * 
                 * Note that since we have not read the next character yet, we
                 * do not know if it exists and hence its position may be
                 * invalid.
                 * 
                 * @return The line of the next character that will be read.
                 */
                std::size_t getLine() const;

                bool popMark() override;
                void pushMark() override;
                bool read(char *dst) override;

                /**
                 * Attempts to read a character from this stream into the
                 * specified destination and store its coordinates in the
                 * corresponding paramters.
                 *
                 * @param dst       The destination of the read element. If no
                 *                  element has been read, e.g. due to
                 *                  end-of-stream, the value pointed to by the
                 *                  specified pointer is implementation-specific.
                 *
                 * @param column    A pointer to some memory where the line of
                 *                  the read character will be stored.
                 *                  If the read is unsuccessful, i.e. the
                 *                  method returns false, the value stored at
                 *                  this pointer is implementation-specific.
                 *
                 * @param column    A pointer to some memory where the column
                 *                  of the read character will be stored.
                 *                  If the read is unsuccessful, i.e. the
                 *                  method returns false, the value stored at
                 *                  this pointer is implementation-specific.
                 *
                 * @return  Either true, if the read operation succeeds and an
                 *          character and its coordinates have been stored at
                 *          the corresponding parameters, or otherwise false,
                 *          if either an error occurred or the end of the
                 *          stream has been reached.
                 */
                bool read(char *dst, std::size_t *line, std::size_t *column);

                bool resetToMark() override;

            private:
                const std::shared_ptr<IBufferedInputStream<char>> source;
                util::Optional<std::size_t> lastColumn;
                util::Optional<std::size_t> lastLine;
                std::size_t nextColumn;
                std::size_t nextLine;
                std::vector<MetricSnapshot> markedMetrics;
            };
        }
    }
}
