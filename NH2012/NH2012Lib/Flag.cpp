#include "Flag.h"


Flag::Flag(void)
{
  debug = false;
}

Flag::~Flag(void)
{
}


void Flag::parseIni(std::string filename)
{
  std::cout << "Getting initialisation data..." << std::endl;

  try
  {
    std::ifstream s(filename);

    if(!s)
    {
      std::cout << "Error opening file" << std::endl;
      return;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(s, pt);

    //General
    enableCollision = (pt.get<std::string>("General.EnableCollision") == "true");
    enableGravity = (pt.get<std::string>("General.EnableGravity") == "true");
    enableAI = (pt.get<std::string>("General.EnableAI") == "true");
    enableAudio = (pt.get<std::string>("General.EnableAudio") == "true");

    hardcore = (pt.get<std::string>("General.HardcoreMode") == "true");
    wizard = (pt.get<std::string>("General.WizardMode") == "true");
    explore = (pt.get<std::string>("General.ExploreMode") == "true");
  
    //Controls //temp default values
    controls.moveForward = OIS::KC_W;
    controls.moveLeft = OIS::KC_A;
    controls.moveBack = OIS::KC_S;
    controls.moveRight = OIS::KC_D;

    controls.jump = OIS::KC_SPACE;
    controls.kick = OIS::KC_F;
    controls.run = OIS::KC_LSHIFT;
    controls.leftHand = OIS::MB_Left;
    controls.rightHand = OIS::MB_Right;

    controls.quickslots.push_back(OIS::KC_1);
    controls.quickslots.push_back(OIS::KC_2);
    controls.quickslots.push_back(OIS::KC_3);
    controls.quickslots.push_back(OIS::KC_4);
    controls.quickslots.push_back(OIS::KC_5);
    controls.quickslots.push_back(OIS::KC_6);
    controls.quickslots.push_back(OIS::KC_7);
    controls.quickslots.push_back(OIS::KC_8);
    controls.quickslots.push_back(OIS::KC_9);
    controls.quickslots.push_back(OIS::KC_0);

    controls.exit = OIS::KC_ESCAPE;

    controls.freezeCollision = OIS::KC_Q;

    //Environment
    enableHDR = (pt.get<std::string>("Environment.HDR") == "true");
    enableBloom = (pt.get<std::string>("Environment.Bloom") == "true");
    enableShadows = (pt.get<std::string>("Environment.Shadows") == "true");
    enableLights = (pt.get<std::string>("Environment.Lights") == "true");
    enableParticles = (pt.get<std::string>("Environment.Particles") == "true");
    enableDecals = (pt.get<std::string>("Environment.Decals") == "true");
    enableSprites = (pt.get<std::string>("Environment.Sprites") == "true");
    enableWater = (pt.get<std::string>("Environment.Water") == "true");
    enableSky = (pt.get<std::string>("Environment.Sky") == "true");

    //Monsters
    monsterDataFilename = pt.get<std::string>("Monsters.Data");

    //Items
    itemDataFilename = pt.get<std::string>("Items.Data");

    //Debug
    debug = (pt.get<std::string>("Debug.DebugMode") == "true");
    verbose = (pt.get<std::string>("Debug.VerboseMode") == "true");

    freeCameraDebug = (pt.get<std::string>("Debug.FreeCamera") == "true");
    wireframeDebug = (pt.get<std::string>("Debug.Wireframe") == "true");
    freezeCollisionDebug = (pt.get<std::string>("Debug.FreezeCollision") == "true");
    showCollisionDebug = (pt.get<std::string>("Debug.ShowCollisionsDebug") == "true");
    showShadowDebug = (pt.get<std::string>("Debug.ShowShadowDebug") == "true");

  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)
  {
    std::cout << e.what() << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cout << e.what() << std::endl;
  }
}

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

