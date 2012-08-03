#include "Environment.h"

#include "NHException.h"

Environment::Environment(void)
  : dataManager(new DataManager())
{
  debug = false;
}

Environment::~Environment(void)
{
  if(dataManager) delete dataManager;
  dataManager = 0;
}


void Environment::parseIni(std::string filename)
{
  std::cout << "Getting initialisation data..." << std::endl;

  try
  {
    std::ifstream s(filename);

    if(!s) throw NHException("Error opening initialisation file.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(s, pt);

    std::string TRUE = "true";

    //General
    enablePhysics = (pt.get<std::string>("General.EnablePhysics") == TRUE);
    enableAI = (pt.get<std::string>("General.EnableAI") == TRUE);
    enableAudio = (pt.get<std::string>("General.EnableAudio") == TRUE);

    gravity = pt.get<float>("General.Gravity");

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
    controls.addItem = OIS::KC_1;
    controls.addMonster = OIS::KC_2;
    controls.reset = OIS::KC_3;

    controls.console = OIS::KC_GRAVE;

    //Environment
    enableHDR = (pt.get<std::string>("Environment.HDR") == TRUE);
    enableBloom = (pt.get<std::string>("Environment.Bloom") == TRUE);
    enableShadows = (pt.get<std::string>("Environment.Shadows") == TRUE);
    enableLights = (pt.get<std::string>("Environment.Lights") == TRUE);
    enableParticles = (pt.get<std::string>("Environment.Particles") == TRUE);
    enableDecals = (pt.get<std::string>("Environment.Decals") == TRUE);
    enableSprites = (pt.get<std::string>("Environment.Sprites") == TRUE);
    enableWater = (pt.get<std::string>("Environment.Water") == TRUE);
    enableSky = (pt.get<std::string>("Environment.Sky") == TRUE);

    //Scenes
    sceneDataFilename = pt.get<std::string>("Scenes.Data");
    dataManager->addData(sceneDataFilename);

    //Architecture
    architectureDataFilename = pt.get<std::string>("Architecture.Data");
    dataManager->addData(architectureDataFilename);

    //Monsters
    monsterDataFilename = pt.get<std::string>("Monsters.Data");
    dataManager->addData(monsterDataFilename);

    //Items
    itemDataFilename = pt.get<std::string>("Items.Data");
    dataManager->addData(itemDataFilename);

    //Debug
    debug = (pt.get<std::string>("Debug.DebugMode") == TRUE);
    verbose = (pt.get<std::string>("Debug.VerboseMode") == TRUE);

    freeCameraDebug = (pt.get<std::string>("Debug.FreeCamera") == TRUE);
    wireframeDebug = (pt.get<std::string>("Debug.Wireframe") == TRUE);
    freezeCollisionDebug = (pt.get<std::string>("Debug.FreezeCollision") == TRUE);
    showCollisionDebug = (pt.get<std::string>("Debug.ShowCollisionsDebug") == TRUE);
    showShadowDebug = (pt.get<std::string>("Debug.ShowShadowDebug") == TRUE);

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

DataManager* Environment::getDataManager()
{
  return dataManager;
}

Date Environment::getDate()
{
  return Date();
}

Ogre::String Environment::getDateString()
{
  return serialiseDate(Date());
}

Ogre::String Environment::getMoonPhase()
{
  return serialiseMoonPhase(calculateMoonPhase(Date()));
}

Ogre::String Environment::serialiseDate(Date date)
{
  std::stringstream dateString;
  dateString << date.day << "/" << date.month << "/" << date.year;
  return (dateString.str());
}

bool Environment::isFriday13()
{
  Date date = Date();
  return ((day(date) == "friday") && (date.day == 13));
}

bool Environment::isBirthday()
{
  Date date = Date();
  return (date.month == 10 && date.day == 30);
}

bool Environment::isHalloween()
{
  Date date = Date();
  return (date.month == 10 && date.day == 31);
}

bool Environment::isNewYears()
{
  Date date = Date();
  return (date.month == 1 && date.day == 1);
}

bool Environment::isDebug()
{
  return debug;
}

int Environment::calculateMoonPhase(Date date)
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
  c = boost::lexical_cast<int>(365.25 * date.year);
  e = boost::lexical_cast<int>(30.6 * date.month);
  jd = c + e + date.day - 694039.09;  /* jd is total days elapsed */
  jd /= 29.53;           /* divide by the moon cycle (29.53 days) */
  b = boost::lexical_cast<int>(jd);		   /* int(jd) -> b, take integer part of jd */
  jd -= b;		   /* subtract integer part to leave fractional part of original jd */
  b = boost::lexical_cast<int>(jd * 8 + 0.5);	   /* scale fraction from 0-8 and round by adding 0.5 */
  b = b & 7;		   /* 0 and 8 are the same so turn 8 into 0 */
  
  return b;
}

Ogre::String Environment::serialiseMoonPhase(int moonPhase)
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

Ogre::String Environment::day(Date date)
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  date.year -= date.month < 3;
  int numerical = (date.year + date.year / 4 - date.year / 100 + date.year / 400 + t[date.month - 1] + date.day) % 7;

  Ogre::String days[] = {"monday","tuesday","wednesday","thursday","friday","saturday","sunday"};

  if(numerical > 6 || numerical < 0) return "error";
  return days[numerical - 1];
}

Ogre::String Environment::month(Date date)
{
  Ogre::String months[] = {"january","february","march","april","may","june","july","august","september","october","november","december"};

  if(date.month < 0 || date.month > 11) return "error";
  return months[date.month - 1];
}
