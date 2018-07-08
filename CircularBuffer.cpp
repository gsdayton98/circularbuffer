#include "CircularBuffer.h"

// Create an empty circular buffer at least 'nsize' big.
CircularBuffer::CircularBuffer(size_t nsize)
: bufferCapacity{CircularBuffer::roundup(nsize)},
  buffer{new uintptr_t[bufferCapacity]},
  bufferCapacityMinus1(bufferCapacity - 1UL)
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


// Get the index of the entry after entry i.
size_t CircularBuffer::next(size_t i) const {
  return (i + 1) & bufferCapacityMinus1;
}
