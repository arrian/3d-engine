// Vector.h
//
// INTERFACE: Class for 3-vectors with double precision
//
// (c) 1996 Christopher Kline <ckline@acm.org>


#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>
#include <stdarg.h>
#include <iostream>
#include <stdlib.h>

namespace Boids
{


// ---------------- VECTORS CLASS ------------------
 
class Vector {

public:
  double x, y, z;                         // direction with magnitude
  
  Vector(void);                           // Default constructor
  Vector( double a, double b, double c);  // Constructor
  
  void Set(double a, double b, double c);  // Set each component of the vector
  void SetDirection(const Vector &d);      // Set the direction of the vector without modifying the length
  void CopyDirection(const Vector &d);     // Set the direction of this vector to be the same as the direction of the argument
  void SetMagnitude(const double m);       // Set the magnitude of the vector without modifying the direction
  void CopyMagnitude(const Vector &v);     // Set the magnitude of this vector to be the same as the magnitude of the argument
  void Normalize(void);                    // Normalize the vector to have a length of 1
  double Length(void);			   // Return the magnitude (length) of this vector 
  
  Vector &operator=(const Vector &b);                                // ex: a = b
  double &operator[](const int index);                               // ex: a[1] (same as a.y)
  friend int operator!=(const Vector &a, const Vector &b);     // ex: a != b
  friend int operator==(const Vector &a, const Vector &b);     // ex: a == b
  friend Vector operator+(const Vector &a, const Vector &b);   // ex: a + b
  friend Vector operator-(const Vector &a, const Vector &b);   // ex: a - b
  friend Vector operator-(const Vector &a);                    // ex: -a
  friend Vector &operator+=(Vector &a, const Vector &b);             // ex: a += b
  friend Vector &operator-=(Vector &a, const Vector &b);             // ex: a -= b
  friend Vector operator%(const Vector &a, const Vector &b);   // ex: a % b (cross product)
  friend double operator*(const Vector &a, const Vector &b);   // ex: a * b (dot product)
  friend Vector operator*(const double &a, const Vector &b);   // ex: a * b (scalar multiplication)
  friend Vector operator*(const Vector &a, const double &b);   // ex: a * b (scalar multiplication)
  friend Vector &operator*=(Vector &a, const double &b);             // ex: a *= b (scalar multiplication + assignment)
  friend Vector operator/(const Vector &a, const double &b);   // ex: a / b (scalar divide)
  friend Vector &operator/=(Vector &a, const double &b);             // ex: a /= b (scalar divide + assignment)

  friend double Magnitude(const Vector &a);                    // Returns the length of the argument
  friend double AngleBetween(const Vector &a, const Vector &b);// Returns the angle (in radians!) between the two arguments
  
private:

};



// ------------ CALCULATIONS USING VECTORS --------------

Vector Direction(const Vector &a);
Vector Direction(const double &x, const double &y, const double &z);
Vector Average(int numvectors, Vector a, ...);

// --------------- I/O OPERATORS ------------------------

std::ostream &operator<<(std::ostream &strm, const Vector &v);


//-----------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------

#define VLENSQRD(a, b, c) ((a)*(a) + (b)*(b) + (c)*(c))
#define VLEN(a, b, c) sqrt(VLENSQRD(a, b, c))

inline void 
Vector::Set(double a, double b, double c) {

  x = a;
  y = b;
  z = c;

}

inline void 
Vector::Normalize(void) {

  double mag = VLEN(x, y, z);

  if (mag == 0)
    return;

  x /= mag;
  y /= mag;
  z/= mag;
  
}

inline Vector::Vector(void) {

  Set(0, 0, 0);
}         

inline Vector::Vector( double a, double b, double c) { 

  Set(a, b, c);  
} 

inline Vector &
Vector::operator=(const Vector &b) { // example: a = b

  x = b.x; 
  y = b.y; 
  z = b.z; 

  return(*this);
}

inline void 
Vector::SetMagnitude(const double m) {

  this->Normalize();
  *this *= m;

}

inline void 
Vector::CopyMagnitude(const Vector &v) {

 SetMagnitude(Magnitude(v));
}

inline void 
Vector::SetDirection(const Vector &d) {

  double m = Magnitude(*this);
  Vector v = d;
  
  v.Normalize();
  *this = v * m;
  
}

inline void 
Vector::CopyDirection(const Vector &d) {

  SetDirection(Direction(d));
}

inline double
Vector::Length(void) {

  return Magnitude(*this);
}

inline double &
Vector::operator[](const int index) {  // example: a[1] (same as a.y)

  if (index == 0) 
    return(x);
  else if (index == 1) 
    return(y);
  else if (index == 2) 
    return(z);
  else {
    std::cerr << "WARNING: You're using subscripting to access a nonexistant vector component (one other than x, y, or z). THIS IS BAD!!\n";
    exit(888);
    return(z); // this will never get called, but prevents compiler warnings...
  }
  
}

inline int 
operator!=(const Vector &a, const Vector &b) { // example: a 1= b

  return(a.x != b.x || a.y != b.y || a.z != b.z);
}

inline int 
operator==(const Vector &a, const Vector &b) { // example: a == b

  return(a.x == b.x && a.y == b.y && a.z == b.z);
}

inline Vector 
operator+(const Vector &a, const Vector &b) { // example: a + b

  Vector c(a.x+b.x, a.y+b.y, a.z+b.z);
  return(c);
}

inline Vector 
operator-(const Vector &a, const Vector &b) { // example: a - b

  Vector c(a.x-b.x, a.y-b.y, a.z-b.z);
  return(c);
}

inline Vector 
operator-(const Vector &a) { // example: -a

  Vector c(-a.x, -a.y, -a.z);

  return(c);
}

inline Vector &
operator+=(Vector &a, const Vector &b) { // example: a += b

  a = a + b;
  return(a);
}

inline Vector &
operator-=(Vector &a, const Vector &b) { // example: a -= b

  a = a - b;
  return(a);
}

inline Vector 
operator%(const Vector &a, const Vector &b) { // example: a % b (cross product)

  Vector c(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  return(c);
}

inline double 
operator*(const Vector &a, const Vector &b) { // example: a * b (dot product)

  return(a.x*b.x + a.y*b.y + a.z*b.z);
}

inline Vector 
operator*(const double &a, const Vector &b) { // example: a * b (scalar multiplication)

  Vector c = b;
  c.x *= a;
  c.y *= a;
  c.z *= a;
  
  return(c);
}

inline Vector 
operator*(const Vector &a, const double &b) { // example: a * b (scalar multiplication)

  return(b * a);
}

inline Vector &
operator*=(Vector &a, const double &b) { // example: a *= b (scalar multiplication + assignment)

  a = a * b;
  return(a);
}

inline Vector 
operator/(const Vector &a, const double &b) { // example: a / b (scalar divide)

  if (b == 0) 
    std::cerr << "WARNING: You're dividing a vector by a zero-length scalar! NOT GOOD!\n";

  Vector c = a*(1/b);

  return(c);
}

inline Vector &
operator/=(Vector &a, const double &b) { // example: a / b (scalar divide + assignment)

  a = a/b;
  return(a);
}

inline std::ostream &
operator<<(std::ostream &strm, const Vector &v) {

  return strm << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

inline Vector 
Direction(const Vector &a) {

  Vector u = a;

  u.Normalize();
  return(u);
}

inline Vector
Direction(const double &x, const double &y, const double &z) {

  return Direction(Vector(x, y, z));
}


inline double 
Magnitude(const Vector &a) {

  return VLEN(a.x, a.y, a.z);
}

inline double 
AngleBetween(const Vector &a, const Vector &b) {

  // returns angle between a and b in RADIANS
  return acos((a*b)/(Magnitude(a)*Magnitude(b)));
} 

}

#undef VLEN
 
#endif /* #ifndef __VECTOR_H */
