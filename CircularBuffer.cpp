#include "CircularBuffer.hpp"

// Create an empty circular buffer at least 'nsize' big.
CircularBuffer::CircularBuffer(size_t nsize)
: bufferCapacity{CircularBuffer::roundup(nsize)},
  buffer{new uintptr_t[bufferCapacity] },
  bufferCapacityMinus1{bufferCapacity - 1UL},
  head{0UL},
  tail{0UL},
  mtx {},
  cv{}
{
}


CircularBuffer::~CircularBuffer(void) {
  delete[] buffer;
}


// round-up n to the next power of 2
// Having a buffer whose size is a power of 2 makes modulo operations
// very easy
size_t CircularBuffer::roundup(size_t n) {
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  n += (n == 0);
  return n;
}



bool CircularBuffer::tryput(uintptr_t x) {
  bool status = false;
  if (mtx.try_lock()) {
    if (!full()) {
      buffer[head] = x;
      head = next(head);
      status = true;
    }
    mtx.unlock();
  }
  return status;
}


bool CircularBuffer::tryget(uintptr_t& x) {
  bool status = false;

  if (mtx.try_lock()) {
    if (!empty()) {
      x = buffer[tail];
      tail = next(tail);
      status = true;
    }
    mtx.unlock();
  }

  return status;
}


using std::unique_lock;
using std::mutex;

void CircularBuffer::put(uintptr_t x) {
  unique_lock<mutex> lock(mtx);
  cv.wait(lock, [this] { return !full(); });
  buffer[head] = x;
  head = next(head);

  lock.unlock();
  cv.notify_all();
}


uintptr_t CircularBuffer::get(void) {
  unique_lock<mutex> lock(mtx);
  cv.wait(lock, [ this ] { return !empty(); });
  uintptr_t val = buffer[tail];
  tail = next(tail);

  lock.unlock();
  cv.notify_all();

  return val;
}
