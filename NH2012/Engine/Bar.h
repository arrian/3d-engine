#pragma once

#include <OgreMath.h>

class Bar
{
public:
  Bar(void);
  Bar(Ogre::Real maxValue);
  Bar(Ogre::Real maxValue, Ogre::Real currentValue);
  ~Bar(void);

  /*! Returns true if the bar is empty.*/
  bool isEmpty();

  /*! Returns true if the bar is full.*/
  bool isFull();

  /*! Gets the fraction of the bar that is full.*/
  Ogre::Real getFraction();

  Ogre::Real getMax();
  Ogre::Real getCurrent();

  void setCurrent(Ogre::Real current);
  void setMax(Ogre::Real max);

  void setFull();
  void setEmpty();

private:
  /*! Maximum value of the bar.*/
  Ogre::Real max;

  /*! Current value of the bar.*/
  Ogre::Real current;
};

