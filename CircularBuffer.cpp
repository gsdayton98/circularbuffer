#include "CircularBuffer.hpp"

// Create an empty circular buffer at least 'nsize' big.
CircularBuffer::CircularBuffer(size_t nsize)
: bufferCapacity{CircularBuffer::roundup(nsize)},
  buffer{new uintptr_t[bufferCapacity]},
  bufferCapacityMinus1(bufferCapacity - 1UL),
  head(0UL),
  tail(0UL)
{
}


CircularBuffer::~CircularBuffer(void) {
  delete[] buffer;
}


bool CircularBuffer::empty(void) const { return head == tail; }

bool CircularBuffer::full(void) const { return next(head) == tail; }

size_t CircularBuffer::size(void) const {
  return (head + bufferCapacity - tail) & bufferCapacityMinus1;
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


// Get the index of the entry after entry i.
size_t CircularBuffer::next(size_t i) const {
  return (i + 1) & bufferCapacityMinus1;
}


bool CircularBuffer::tryput(uintptr_t x) {
  bool status = false;
  if (! full()) {
    buffer[head] = x;
    head = next(head);
    status = true;
  }
  return status;
}


bool CircularBuffer::tryget(uintptr_t &x) {
  bool status = false;

  if (!empty()) {
    x = buffer[tail];
    tail = next(tail);
    status = true;
  }
  return status;
}
