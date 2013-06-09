

#include <Vector3.h>
#include <Quaternion.h>

class Physical
{

public:
  virtual void setPostion(Vector3 position) = 0;
  virtual Vector3 getPostion() = 0;

  virtual void setRotation(Quaternion rotation) = 0;
  virtual Quaternion getRotation() = 0;

};