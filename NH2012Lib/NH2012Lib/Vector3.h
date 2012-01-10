#pragma once
class Vector3
{
public:
  Vector3(double x, double y, double z);
  virtual ~Vector3(void);

  double X();
  double Y();
  double Z();
private:
  double x;
  double y;
  double z;
};

