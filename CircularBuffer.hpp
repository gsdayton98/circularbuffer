#ifndef CIRCULARBUFFER_HPP_INCL
#define CIRCULARBUFFER_HPP_INCL
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
  /// <summary>Create an empty circular buffer at least 'nsize' big.
  ///   Actual size of the buffer will be rounded up to a power of 2.</summary>
  ///
  /// <param name="nsize">Minimum size of the buffer</param>
  explicit CircularBuffer(size_t nsize);

   /// Deallocate the buffer
  ~CircularBuffer(void);

  /// <summary>Return the capacity of the buffer</summary>
  ///
  /// <returns>size_t  Capacity of the buffer</returns>
  size_t capacity(void) const { return bufferCapacity; }

  /// <summary>Predicate for testing whether buffer is empty</summary>
  /// </returns> true if buffer is empty
  bool empty(void) const;

  /// <summary>Predicate for testing whether buffer is full</summary>
  /// </returns> true if buffer is full
  bool full(void) const;

  /// <summary>returns number of elements in the buffer
  /// </returns>number of elements in the buffer
  size_t size(void) const;

  bool tryput(uintptr_t);

  bool tryget(uintptr_t &);

protected:
  static size_t roundup(size_t n);

  size_t next(size_t i) const;


private:
  const size_t bufferCapacity;
  uintptr_t * const buffer;
  const size_t bufferCapacityMinus1;
  size_t head;
  size_t tail;

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;
};

#endif // CIRCULARBUFFER_HPP_INCL
