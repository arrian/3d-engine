#include "Flag.h"


Flag::Flag(void)
{
  debug = false;
}

Flag::~Flag(void)
{
}

/*
void Flag::parseIni()
{
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini("nh2012.ini", pt);

  enableCollision = (pt.get<std::string>("General.EnableCollision") == "true");
  enablePhysics = (pt.get<std::string>("General.EnablePhysics") == "true");
  enableAI = (pt.get<std::string>("General.EnableCollision") == "true");
  enableAudio = (pt.get<std::string>("General.EnableCollision") == "true");
  
  debug = (pt.get<std::string>("General.EnableCollision") == "true");
  verbose = (pt.get<std::string>("General.VerboseLogging") == "true");
  hardcore = (pt.get<std::string>("Player.HardcoreMode") == "true");
  wizard = (pt.get<std::string>("Player.WizardMode") == "true");
  explore = (pt.get<std::string>("Player.ExploreMode") == "true");

  hdr = (pt.get<std::string>("Environment.HDR") == "true");
  bloom = (pt.get<std::string>("Environment.Bloom") == "true");
  shadows = (pt.get<std::string>("Environment.Shadows") == "true");
  lights = (pt.get<std::string>("Environment.Lights") == "true");
  particles = (pt.get<std::string>("Environment.Particles") == "true");
  decals = (pt.get<std::string>("Environment.Decals") == "true");
  sprites = (pt.get<std::string>("Environment.Sprites") == "true");

  monsterDataFilename = pt.get<std::string>("Monsters.DataFile");
  itemDataFilename = pt.get<std::string>("Items.DataFile");
}
*/

Date Flag::getDate()
{
  return Date();
}

Ogre::String Flag::getDateString()
{
  return serialiseDate(Date());
}

Ogre::String Flag::getMoonPhase()
{
  return serialiseMoonPhase(calculateMoonPhase(Date()));
}

Ogre::String Flag::serialiseDate(Date date)
{
  std::stringstream dateString;
  dateString << dayOfWeek(date) << " " << date.day << "/" << date.month << "/" << date.year;
  return (dateString.str()); 
}

bool Flag::isFriday13()
{
  Date date = Date();
  return ((dayOfWeek(date) == "friday") && (date.day == 13));
}

bool Flag::isBirthday()
{
  Date date = Date();
  return (date.month == 10 && date.day == 30);
}

bool Flag::isHalloween()
{
  Date date = Date();
  return (date.month == 10 && date.day == 31);
}

bool Flag::isNewYears()
{
  Date date = Date();
  return (date.month == 1 && date.day == 1);
}

bool Flag::isDebug()
{
  return debug;
}

Ogre::Real Flag::calculateMoonPhase(Date date)
{
  /*
  calculates the moon phase (0-7), accurate to 1 segment.
  0 = > new moon.
  4 => full moon.
  */

  int c,e;
  double jd;
  int b;

  if (date.month < 3) {
      date.year--;
      date.month += 12;
  }
  ++date.month;
  c = 365.25*date.year;
  e = 30.6*date.month;
  jd = c+e+date.day-694039.09;  /* jd is total days elapsed */
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

Ogre::String Flag::dayOfWeek(Date date)
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  date.year -= date.month < 3;
  int numerical = (date.year + date.year/4 - date.year/100 + date.year/400 + t[date.month-1] + date.day) % 7;

  Ogre::String days[] = {"monday","tuesday","wednesday","thursday","friday","saturday","sunday"};

  if(numerical > 6 || numerical < 0) return "error";
  return days[numerical - 1];
}

