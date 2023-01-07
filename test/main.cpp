#include <cppunit/ui/text/TestRunner.h>
#include "service/io/MosaicInputStreamTest.h"

using namespace CppUnit::TextUi;

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    TestRunner runner;
    runner.addTest(c4::service::io::MosaicInputStreamTest::suite());
    runner.run();
    return 0;
}
