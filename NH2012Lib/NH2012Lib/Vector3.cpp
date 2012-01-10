#include "Vector3.h"


Vector3::Vector3(double x, double y, double z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3::~Vector3(void)
{
}

double Vector3::X()
{
  return x;
}

double Vector3::Y()
{
  return y;
}

double Vector3::Z()
{
  return z;
}