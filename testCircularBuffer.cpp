#include <cstdlib>
#include <CppUnitXLite/CppUnitXLite.cpp>
#include <utility>
#include "CircularBuffer.h"

namespace {
 // Inherit from CircularBuffer so we can test the internal functions.
 struct TesterCircularBuffer : public CircularBuffer {
  static size_t roundup(size_t n) { return CircularBuffer::roundup(n); }

  explicit TesterCircularBuffer(size_t n) : CircularBuffer{n} { }

  size_t next(size_t i) { return CircularBuffer::next(i); }
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

TEST(TestCircularBuffer, next) {
  TesterCircularBuffer cbuf{8};

  for (size_t i = 0; i < 2 * cbuf.capacity(); ++i) {
    size_t j = cbuf.next(i);
    CHECK_EQUAL((i + 1UL) % cbuf.capacity(), j);
  }
}

TESTMAIN
