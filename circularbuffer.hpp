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
template <typename ElementType>
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

  /**
   * Try putting an element into the buffer
   * @return bool true if successful
   */
  bool tryput(ElementType value);

  /**
   * Try getting a value from the buffer
   * @return bool true on success
   */
  bool tryget(ElementType& value);

  /**
   * Put a value into the buffer.  Blocks until it is done.
   * @param value Value to insert into buffer
   */
  void put(ElementType value);

  /**
   * Get a value from the buffer.  Blocks until it is done.
   * @return ElementType
   */
  ElementType get(void);

protected:
  // Utility for rounding up the size of the buffer to a power of 2 to
  // simplify modulo operation
  static size_t roundup(size_t n);

  // Utility for getting the index of the next element in the buffer,
  // modulo the size of the buffer.
  inline size_t next(size_t i) const;

private:
  const size_t bufferCapacity;
  ElementType *const buffer;
  const size_t bufferCapacityMinus1;
  volatile size_t head;
  volatile size_t tail;

  // Protects head and tail
  std::mutex mtx;
  std::condition_variable cv;

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;
};


template<typename ElementType>
CircularBuffer<ElementType>::CircularBuffer(size_t nsize)
   : bufferCapacity{CircularBuffer::roundup(nsize)},
     buffer{new ElementType[bufferCapacity] },
     bufferCapacityMinus1{bufferCapacity - 1UL},
     head{0UL},
     tail{0UL},
     mtx{},
     cv{}
{
}


template<typename ElementType>
CircularBuffer<ElementType>::~CircularBuffer(void) {
  delete[] buffer;
}


template<typename ElementType>
bool CircularBuffer<ElementType>::empty(void) const { return head == tail; }


template<typename ElementType>
bool CircularBuffer<ElementType>::full(void) const { return next(head)== tail; }


template<typename ElementType>
size_t CircularBuffer<ElementType>::next(size_t i) const {
  return (i + 1) & bufferCapacityMinus1;
}


template<typename ElementType>
size_t CircularBuffer<ElementType>::size(void) const {
  return (head + bufferCapacity - tail) & bufferCapacityMinus1;
}


template<typename ElementType>
bool CircularBuffer<ElementType>::tryput(ElementType x) {
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


template<typename ElementType>
bool CircularBuffer<ElementType>::tryget(ElementType& x) {
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


// round-up n to the next power of 2
// Having a buffer whose size is a power of 2 makes modulo operations
// very easy
template<typename ElementType>
size_t CircularBuffer<ElementType>::roundup(size_t n) {
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


using std::unique_lock;
using std::mutex;

template<typename ElementType>
void CircularBuffer<ElementType>::put(ElementType x) {
  unique_lock<mutex> lock(mtx);

  cv.wait(lock, [ this ] { return !full(); });
  buffer[head] = x;
  head = next(head);

  lock.unlock();
  cv.notify_all();
}


template<typename ElementType>
ElementType CircularBuffer<ElementType>::get(void) {
  unique_lock<mutex> lock(mtx);

  cv.wait(lock, [ this ] { return !empty(); });
  auto val = buffer[tail];

  tail = next(tail);

  lock.unlock();
  cv.notify_all();

  return val;
}
#endif // CIRCULARBUFFER_HPP_INCL
