#pragma once

#include <ctime>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/ini_parser.hpp>

#include <OgreString.h>

struct Date
{
  Date()
  {
    time_t t = time(0);
    struct tm * now = localtime( & t );
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
  }

  Date(int year, int month, int day)
  {
    this->year = year;
    this->month = month;
    this->day = day;
  }

  int year;
  int month;
  int day;
};

/**
 * Stores the global game state
 */
class Flag
{
public:
  Flag(void);
  virtual ~Flag(void);

  Ogre::String monsterDataFilename;
  Ogre::String itemDataFilename;

  //Dates
  Date getDate();
  Ogre::String getDateString();
  Ogre::String getMoonPhase();
  bool isFriday13();
  bool isBirthday();
  bool isHalloween();
  bool isNewYears();

private:
  //Modes
  bool explore;//explore mode
  bool wizard;//wizard mode
  bool debug;//console visible
  bool verbose;//verbose logging
  bool hardcore;

  //Enabled Features
  bool enablePhysics;
  bool enableCollision;
  bool enableAI;
  bool enableAudio;

  //Graphics
  bool hdr;
  bool bloom;
  bool shadows;
  bool lights;
  bool decals;
  bool particles;
  bool sprites;

  //void parseIni();

  Ogre::String serialiseMoonPhase(Ogre::Real moonPhase);
  Ogre::Real calculateMoonPhase(Date date);
  Ogre::String dayOfWeek(Date date);
  Ogre::String serialiseDate(Date date);
};

