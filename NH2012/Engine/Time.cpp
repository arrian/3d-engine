#include "Time.h"


Time::Time(void)
{
}

//-------------------------------------------------------------------------------------
Time::~Time(void)
{
}

//-------------------------------------------------------------------------------------
Date Time::getDate()
{
  return Date();
}

//-------------------------------------------------------------------------------------
Ogre::String Time::getDateString()
{
  return serialiseDate(Date());
}

//-------------------------------------------------------------------------------------
Ogre::String Time::getMoonPhase()
{
  return serialiseMoonPhase(calculateMoonPhase(Date()));
}

//-------------------------------------------------------------------------------------
Ogre::String Time::serialiseDate(Date date)
{
  std::stringstream dateString;
  dateString << date.day << "/" << date.month << "/" << date.year;
  return (dateString.str());
}

//-------------------------------------------------------------------------------------
bool Time::isFriday13()
{
  Date date = Date();
  return ((day(date) == "friday") && (date.day == 13));
}

//-------------------------------------------------------------------------------------
bool Time::isBirthday()
{
  Date date = Date();
  return (date.month == 10 && date.day == 30);
}

//-------------------------------------------------------------------------------------
bool Time::isHalloween()
{
  Date date = Date();
  return (date.month == 10 && date.day == 31);
}

//-------------------------------------------------------------------------------------
bool Time::isNewYears()
{
  Date date = Date();
  return (date.month == 1 && date.day == 1);
}

//-------------------------------------------------------------------------------------
int Time::calculateMoonPhase(Date date)
{
  /*
  Calculates the moon phase (0-7).
  0 => new moon.
  4 => full moon.
  */

  int c;
  int e;
  double jd;
  int b;

  if (date.month < 3)
  {
      date.year--;
      date.month += 12;
  }

  ++date.month;
  c = boost::lexical_cast<int>(365.25 * date.year);
  e = boost::lexical_cast<int>(30.6 * date.month);
  jd = c + e + date.day - 694039.09;/* jd is total days elapsed */
  jd /= 29.53;/* divide by the moon cycle (29.53 days) */
  b = boost::lexical_cast<int>(jd);/* int(jd) -> b, take integer part of jd */
  jd -= b;/* subtract integer part to leave fractional part of original jd */
  b = boost::lexical_cast<int>(jd * 8 + 0.5);/* scale fraction from 0-8 and round by adding 0.5 */
  b = b & 7;/* 0 and 8 are the same so turn 8 into 0 */
  
  return b;
}

//-------------------------------------------------------------------------------------
Ogre::String Time::serialiseMoonPhase(int moonPhase)
{
  switch(moonPhase)
  {
    case 0: return "new moon";
    case 1: return "waxing crescent moon";
    case 2: return "first quarter moon";
    case 3: return "waxing gibbous moon";
    case 4: return "full moon";
    case 5: return "waning gibbous moon";
    case 6: return "third quarter moon";
    case 7: return "waning crescent moon";
    default: return "new moon";
  }
}

//-------------------------------------------------------------------------------------
Ogre::String Time::day(Date date)
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  date.year -= date.month < 3;
  int numerical = (date.year + date.year / 4 - date.year / 100 + date.year / 400 + t[date.month - 1] + date.day) % 7;

  Ogre::String days[] = {"monday","tuesday","wednesday","thursday","friday","saturday","sunday"};

  if(numerical > 6 || numerical < 0) return "error";
  return days[numerical - 1];
}

//-------------------------------------------------------------------------------------
Ogre::String Time::month(Date date)
{
  Ogre::String months[] = {"january","february","march","april","may","june","july","august","september","october","november","december"};

  if(date.month < 0 || date.month > 11) return "error";
  return months[date.month - 1];
}
