#include "CircularBuffer.h"

// Create an empty circular buffer at least 'nsize' big.
CircularBuffer::CircularBuffer(size_t nsize)
: bufferCapacity{CircularBuffer::roundup(nsize)},
  buffer{new uintptr_t[bufferCapacity]}
{
}


CircularBuffer::~CircularBuffer(void) {
  delete[] buffer;
}



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
