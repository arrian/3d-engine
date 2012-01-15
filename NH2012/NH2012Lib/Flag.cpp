#include "Flag.h"


Flag::Flag(void)
{
}


Flag::~Flag(void)
{
}

Ogre::Real Flag::calculateMoonPhase(int day, int month, int year)
{
  /*
  calculates the moon phase (0-7), accurate to 1 segment.
  0 = > new moon.
  4 => full moon.
  */

  int c,e;
  double jd;
  int b;

  if (month < 3) {
      year--;
      month += 12;
  }
  ++month;
  c = 365.25*year;
  e = 30.6*month;
  jd = c+e+day-694039.09;  /* jd is total days elapsed */
  jd /= 29.53;           /* divide by the moon cycle (29.53 days) */
  b = jd;		   /* int(jd) -> b, take integer part of jd */
  jd -= b;		   /* subtract integer part to leave fractional part of original jd */
  b = jd*8 + 0.5;	   /* scale fraction from 0-8 and round by adding 0.5 */
  b = b & 7;		   /* 0 and 8 are the same so turn 8 into 0 */
  
  return b;
}

Ogre::String Flag::serialiseMoonPhase(Ogre::Real moonPhase)
{
  if(moonPhase == 0) return "new moon.";
  if(moonPhase == 4) return "full moon.";
  if(moonPhase < 4) return "waxing moon.";
  return "waning moon.";
}