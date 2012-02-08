#pragma once

#include <ctime>


#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>
#include <set>
#include <sstream>

#include <OgreString.h>

#include <OISKeyboard.h>
#include <OISMouse.h>

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

struct Controls
{
  OIS::KeyCode moveForward;
  OIS::KeyCode moveLeft;
  OIS::KeyCode moveBack;
  OIS::KeyCode moveRight;

  OIS::KeyCode jump;
  OIS::KeyCode kick;
  OIS::KeyCode run;

  OIS::MouseButtonID leftHand;
  OIS::MouseButtonID rightHand;

  std::vector<OIS::KeyCode> quickslots;

  OIS::KeyCode exit;

  //temp debug controls
  OIS::KeyCode freezeCollision;
  OIS::KeyCode addItem;
  OIS::KeyCode addMonster;
  OIS::KeyCode reset;
};

/**
 * Stores the global game state
 */
class Flag
{
public:
  Flag(void);
  virtual ~Flag(void);

  void parseIni(std::string filename);

  Controls controls;

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

  bool isDebug();

  //Modes
  bool explore;//explore mode
  bool wizard;//wizard mode
  bool hardcore;

  bool debug;
  bool verbose;//verbose logging

  //Enabled Features
  bool enableCollision;
  bool enableGravity;
  bool enableAI;
  bool enableAudio;

  //Graphics
  bool enableHDR;
  bool enableBloom;
  bool enableShadows;
  bool enableLights;
  bool enableParticles;
  bool enableDecals;
  bool enableSprites;
  bool enableWater;
  bool enableSky;

  //Debug booleans
  bool freeCameraDebug;
  bool wireframeDebug;
  bool freezeCollisionDebug;
  bool showCollisionDebug;
  bool showShadowDebug;

private:
  Ogre::String serialiseMoonPhase(Ogre::Real moonPhase);
  Ogre::Real calculateMoonPhase(Date date);
  Ogre::String dayOfWeek(Date date);
  Ogre::String serialiseDate(Date date);

};

