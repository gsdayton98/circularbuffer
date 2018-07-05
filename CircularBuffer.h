#ifndef __CIRCULARBUFFER_H_INCL__
#define __CIRCULARBUFFER_H_INCL__

/**
 * TODO: Add class description
 *
 * @author gdayton
 */
class CircularBuffer {
public:
  // Constructor
  CircularBuffer(void);

  // Destructor
  virtual ~CircularBuffer(void);

  // Copy constructor
  // TODO: Uncomment the copy constructor when you need it.
  //CircularBuffer(const CircularBuffer& src);

  // Assignment operator
  // TODO: Uncomment the assignment operator when you need it.
  //CircularBuffer& operator=(const CircularBuffer& src);
};

// Constructor implementation
inline CircularBuffer::CircularBuffer(void) { }

// Destructor implementation
inline CircularBuffer::~CircularBuffer(void) { }

// TODO: Uncomment the copy constructor when you need it.
//inline CircularBuffer::CircularBuffer(const CircularBuffer& src)
//{
//   // TODO: copy
//}

// TODO: Uncomment the assignment operator when you need it.
//inline CircularBuffer& CircularBuffer::operator=(const CircularBuffer& rhs)
//{
//   if (this == &rhs) {
//      return *this;
//   }
//
//   // TODO: assignment
//
//   return *this;
//}

#endif // __CIRCULARBUFFER_H_INCL__
