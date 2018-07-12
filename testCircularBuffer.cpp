#include <chrono>
#include <CppUnitXLite/CppUnitXLite.cpp>
#include <cstdlib>
#include <thread>
#include <utility>
#include "CircularBuffer.hpp"

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


TEST(TestCircularBuffer, Roundup)
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

TEST(TestCircularBuffer, Construction) {
  CircularBuffer cbuf{7};

  CHECK_EQUAL(8UL, cbuf.capacity());
}

TEST(TestCircularBuffer, Next) {
  TesterCircularBuffer cbuf{8};

  for (size_t i = 0; i < 2 * cbuf.capacity(); ++i) {
    size_t j = cbuf.next(i);
    CHECK_EQUAL((i + 1UL) % cbuf.capacity(), j);
  }
}

TEST(TestCircularBuffer, Zero) {
  CircularBuffer cbuf{0};

  CHECK(cbuf.empty());
  CHECK(cbuf.full());
}

TEST(TestCircularBuffer, Size) {
  CircularBuffer cbuf{8};

  for (uintptr_t x = 1UL; x <= 5UL;  ++x) {
    CHECK(cbuf.tryput(x));
  }
  CHECK_EQUAL(5UL, cbuf.size());
}

TEST(TestCircularBuffer, ReadWrite1) {
  CircularBuffer cbuf{8};

  for (uintptr_t x = 1UL; x <= 5UL;  ++x) {
    CHECK(cbuf.tryput(x));
  }


  uintptr_t justread;
  for (uintptr_t x = 1UL; x <= 5UL;  ++x) {
    CHECK(cbuf.tryget(justread));
    CHECK_EQUAL(x, justread);
  }
}

TEST(TestCircularBuffer, ReadWriteX) {
  CircularBuffer cbuf{8};

  uintptr_t expectedread = 1UL;
  uintptr_t justread;
  for (uintptr_t x = 1UL; x <= 25UL;  ++x) {
    while (! cbuf.tryput(x)) {
      CHECK(cbuf.tryget(justread));
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;

      CHECK(cbuf.tryget(justread));
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;
      CHECK(cbuf.tryget(justread));
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;
    }
  }

  while (cbuf.tryget(justread)) {
    CHECK_EQUAL(expectedread, justread);
    ++expectedread;
  }
  CHECK(cbuf.empty());
}


TEST(TestCircularBuffer, Singlethread) {
  CircularBuffer cbuf{8};

  uintptr_t expectedread = 1UL;
  uintptr_t justread;

  for (uintptr_t x = 1UL; x <= 25UL;  ++x) {
    cbuf.put(x);
    while (cbuf.full()) {
      justread = cbuf.get();
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;

      justread = cbuf.get();
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;

      justread = cbuf.get();
      CHECK_EQUAL(expectedread, justread);
      ++expectedread;
    }
  }

  while (! cbuf.empty()) {
    justread = cbuf.get();
    CHECK_EQUAL(expectedread, justread);
    ++expectedread;
  }
  CHECK(cbuf.empty());
}

using std::chrono::duration;
using std::chrono::milliseconds;


struct TestCircularBufferMultiThreadTest : public Test {
  CircularBuffer cbuf;

  TestCircularBufferMultiThreadTest(void)
     : Test("CppUnitXLiteTest::TestCircularBufferMultiThreadTest"),
       cbuf{8}
{ }


  // CppUnitXLite is single threaded so only one thread may post results
  void testThread(TestResult& result) {
    static const duration PERIOD = milliseconds(30);

    for (uintptr_t expected = 1UL; expected <= 100UL; ++expected) {
      std::this_thread::sleep_for(PERIOD);
      uintptr_t actual = cbuf.get();
      checkEqual(expected, actual, result, __FILE__, __LINE__);
    }
  }

  void otherThread(void) {
    static const duration PERIOD = milliseconds(55);

    for (uintptr_t actual = 1UL; actual <= 100UL; ++actual) {
      cbuf.put(actual);
      std::this_thread::sleep_for(PERIOD);
    }
  }

  void run(TestResult& result) {
    std::thread consumer(&TestCircularBufferMultiThreadTest::testThread, this, std::ref(result));
    std::thread producer(& TestCircularBufferMultiThreadTest::otherThread, this);
    consumer.join();
    producer.join();
  }

} TestCircularBufferMultiThreadTestInstance;



TESTMAIN
