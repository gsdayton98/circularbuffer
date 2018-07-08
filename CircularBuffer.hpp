#ifndef __CIRCULARBUFFER_H_INCL__
#define __CIRCULARBUFFER_H_INCL__
#include <cstdlib>
#include <cstdint>

/**
 * Circular Buffer, a double ended queue that uses a static array and reuses
 * memory.
 *
 * @author gdayton
 */
class CircularBuffer {
public:
  /**
   * Create an empty circular buffer at least 'nsize' big.
   * Actual size of the buffer will be rounded up to a power of 2.
   * @param nsize
   */
  explicit CircularBuffer(size_t nsize);

  ~CircularBuffer(void);

  size_t capacity(void) const { return bufferCapacity; }

protected:
  static size_t roundup(size_t n);

  size_t next(size_t i) const;


private:
  const size_t bufferCapacity;
  const uintptr_t *buffer;
  const size_t bufferCapacityMinus1;
  size_t head;
  size_t tail;

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;
};

#endif // __CIRCULARBUFFER_H_INCL__
