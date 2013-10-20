// Vector.c++
//
// CODE: Class for 3-vectors with double precision
//
// (c) 1996 Christopher Kline <ckline@acm.org>

#include "Vector.h"
 

namespace Boids
{


Vector
Average(int numvectors, Vector a, ...) {

  Vector v = a;
  va_list arg_ptr;
  int i;

  va_start(arg_ptr, a);
  for (i=0; i<numvectors-1; i++) 
    v += va_arg(arg_ptr, Vector);
  va_end(arg_ptr);

  v.x /= numvectors; v.y /= numvectors; v.z /= numvectors;  
  return(v);
}

}

