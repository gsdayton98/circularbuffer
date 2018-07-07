#include <cstdlib>
#include <exception>
#include <iostream>
#include "CircularBuffer.h"

using std::cerr;
using std::cout;
using std::endl;

int main (int argc, char *argv[])
{
  int returnCode = EXIT_FAILURE;
  try {
    CircularBuffer cbuffer{15};

    returnCode = EXIT_SUCCESS;
  }
  catch (const std::exception &ex)
  {
    cerr << "Otherwise uncaught exception caught in main(): " << ex.what() << endl;
  }
  return returnCode;
}

