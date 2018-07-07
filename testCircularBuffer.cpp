#include <cstdlib>
#include <CppUnitXLite/CppUnitXLite.cpp>
#include <utility>
#include "CircularBuffer.h"

namespace {
 // Inherit from CircularBuffer so we can test the internal functions.
 struct TesterCircularBuffer : public CircularBuffer {
   static size_t roundup(size_t n) { return CircularBuffer::roundup(n); }
 };


 struct TestCase {
   size_t arg;
   size_t expected;

   TestCase(size_t theArg, size_t theExpected)
      : arg(theArg), expected(theExpected) {  }
 };
}


TEST(TestCircularBuffer, roundup)
{
  TestCase testCases[] = {
    TestCase {0, 1},
    TestCase {1, 1},
    TestCase {2, 2},
    TestCase {3, 4},
    TestCase {4, 4},
    TestCase {5, 8},
    TestCase {7, 8},
    TestCase{8, 8},
    TestCase{60, 64}
  };

  for (TestCase &aTestCase : testCases) {
    CHECK_EQUAL(aTestCase.expected, TesterCircularBuffer::roundup(aTestCase.arg));
  }
}

TEST(TestCircularBuffer, construction) {
  CircularBuffer cbuf{7};

  CHECK_EQUAL(8UL, cbuf.capacity());
}

TESTMAIN
