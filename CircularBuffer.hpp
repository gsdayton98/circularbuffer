#ifndef CIRCULARBUFFER_HPP_INCL
#define CIRCULARBUFFER_HPP_INCL
#include <cstdlib>
#include <cstdint>
#include <mutex>
#include <condition_variable>

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
   *
   * @param nsize Minimum size of the buffer
   */
  explicit CircularBuffer(size_t nsize);

  /// Deallocate the buffer
  ~CircularBuffer(void);

  /// Return the capacity of the buffer
  inline size_t capacity(void) const { return bufferCapacity; }

  /// Predicate for testing whether buffer is empty
  inline bool empty(void) const;

  /// Predicate for testing whether buffer is full
  inline bool full(void) const;

  /// returns number of elements in the buffer
  inline size_t size(void) const;

  bool tryput(uintptr_t);

  bool tryget(uintptr_t &);

  void put(uintptr_t);

  uintptr_t get(void);

protected:
  static size_t roundup(size_t n);

  inline size_t next(size_t i) const;

private:
  const size_t bufferCapacity;
  uintptr_t * const buffer;
  const size_t bufferCapacityMinus1;
  volatile size_t head;
  volatile size_t tail;

  // Protects head and tail
  std::mutex mtx;
  std::condition_variable cv;

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;
};


bool CircularBuffer::empty(void) const { return head == tail; }

bool CircularBuffer::full(void) const { return next(head) == tail; }

size_t CircularBuffer::next(size_t i) const {
  return (i + 1) & bufferCapacityMinus1;
}


size_t CircularBuffer::size(void) const {
  return (head + bufferCapacity - tail) & bufferCapacityMinus1;
}
#endif // CIRCULARBUFFER_HPP_INCL
