#pragma once

#include <cstdio>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <service/io/MosaicInputStream.h>
#include "QueueInputStream.h"

namespace c4 {
    namespace service {
        namespace io {
            class MosaicInputStreamTest : public CppUnit::TestFixture {
                CPPUNIT_TEST_SUITE(MosaicInputStreamTest);
                CPPUNIT_TEST(testImmediateEOF);
                CPPUNIT_TEST(testLargeRead);
                CPPUNIT_TEST(testLargeReadTwice);
                CPPUNIT_TEST(testReadUnevenWithMultipleMarks);
                CPPUNIT_TEST_SUITE_END();

            public:
                void testImmediateEOF() {
                    // Adjust

                    auto emptyQueue = std::make_unique<std::queue<char>>();
                    auto queueStream =
                        std::make_shared<QueueInputStream<char>>(
                            std::move(emptyQueue)
                        );

                    MosaicInputStream<char> mosaicStream(queueStream, 7);
                    char tmp;

                    // Act

                    auto eofReached = !mosaicStream.read(&tmp);

                    // Assert

                    CPPUNIT_ASSERT(eofReached);
                }

                void testLargeRead() {
                    // Adjust

                    auto queue = std::make_unique<std::queue<char>>();

                    for (int i = 0; i < 1048576; i++) {
                        queue->push('A');
                    }

                    auto queueStream =
                        std::make_shared<QueueInputStream<char>>(
                            std::move(queue)
                        );

                    MosaicInputStream<char> mosaicStream(queueStream, 1024);
                    std::queue<char> result{};
                    char tmp;

                    // Act

                    while (mosaicStream.read(&tmp)) {
                        result.push(tmp);
                    }

                    // Assert

                    CPPUNIT_ASSERT(1048576 == result.size());

                    while (!result.empty()) {
                        CPPUNIT_ASSERT('A' == result.front());
                        result.pop();
                    }
                }

                void testLargeReadTwice() {
                    // Adjust

                    auto queue = std::make_unique<std::queue<char>>();

                    for (int i = 0; i < 1048576; i++) {
                        queue->push('B');
                    }

                    auto queueStream =
                        std::make_shared<QueueInputStream<char>>(
                            std::move(queue)
                        );

                    MosaicInputStream<char> mosaicStream(queueStream, 1024);
                    std::queue<char> result1{};
                    std::queue<char> result2{};
                    char tmp;

                    // Act

                    mosaicStream.pushMark();

                    while (mosaicStream.read(&tmp)) {
                        result1.push(tmp);
                    }

                    mosaicStream.resetAndPopMark();

                    while (mosaicStream.read(&tmp)) {
                        result2.push(tmp);
                    }

                    // Assert

                    CPPUNIT_ASSERT(1048576 == result1.size());
                    CPPUNIT_ASSERT(1048576 == result2.size());

                    while (!result1.empty() || !result2.empty()) {
                        CPPUNIT_ASSERT('B' == result1.front());
                        CPPUNIT_ASSERT('B' == result2.front());

                        result1.pop();
                        result2.pop();
                    }
                }

                void testReadUnevenWithMultipleMarks() {
                    // Adjust

                    auto expectedResult = "012333456456";
                    auto queue = std::make_unique<std::queue<char>>();

                    for (char c = '0'; c < '7'; c++) {
                        queue->push(c);
                    }

                    auto queueStream =
                        std::make_shared<QueueInputStream<char>>(
                            std::move(queue)
                        );

                    MosaicInputStream<char> mosaicStream(queueStream, 3);
                    std::queue<char> result{};
                    char tmp;

                    // Act

                    mosaicStream.pushMark();
                    mosaicStream.pushMark();

                    for (int i = 0; i < 3; i++) {
                        mosaicStream.pushMark();
                        mosaicStream.read(&tmp);
                        result.push(tmp);
                        mosaicStream.popMark();
                    }

                    mosaicStream.popMark();
                    mosaicStream.moveMark();

                    for (int i = 0; i < 3; i++) {
                        mosaicStream.resetToMark();
                        mosaicStream.read(&tmp);
                        result.push(tmp);
                    }

                    mosaicStream.moveMark();

                    while (mosaicStream.read(&tmp)) {
                        result.push(tmp);
                    }

                    mosaicStream.resetAndPopMark();

                    while (mosaicStream.read(&tmp)) {
                        result.push(tmp);
                    }

                    // Assert

                    CPPUNIT_ASSERT(12 == result.size());

                    for (int i = 0; i < 12; i++) {
                        CPPUNIT_ASSERT(
                            expectedResult[i] == result.front()
                        );

                        result.pop();
                    }
                }
            };
        }
    }
}
