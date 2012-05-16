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

#include "DataManager.h"

/*! Structure for storing dates.*/
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

/*! Structure for storing controls.*/
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

  OIS::KeyCode console;
};

/*! Store the global game state.*/
class Environment
{
public:
  Environment(void);
  virtual ~Environment(void);

  /*! Parses the game settings files.*/
  void parseIni(std::string filename);

  DataManager* getDataManager();

  /*! Stores the control mapping.*/
  Controls controls;

  Ogre::String defaultStartLevel;

  /*! Architecture data filename.
      Default is architecture.dat
  */
  Ogre::String architectureDataFilename;

  /*! Monster data filename.
      Default is monsters.dat
  */
  Ogre::String monsterDataFilename;
  
  /*! Item data filename.
      Default is items.dat
  */
  Ogre::String itemDataFilename;

  /*! Level data filename.
      Default is levels.dat
  */
  Ogre::String levelDataFilename;

  /*! Gets the current date.*/
  Date getDate();

  /*! Gets the current date as a string.*/
  Ogre::String getDateString();

  /*! Gets the current moon phase as a string.*/
  Ogre::String getMoonPhase();

  /*! Returns true if today is Friday the 13th.*/
  bool isFriday13();

  /*! Returns true if today is one of the developer's birthdays.*/
  bool isBirthday();

  /*! Returns true if today is halloween.*/
  bool isHalloween();

  /*! Returns true if today is new years day.*/
  bool isNewYears();

  /*! Returns true if game is in debug mode.*/
  bool isDebug();

  /*! True if debug mode is activated.*/
  bool debug;

  /*! True if verbose logging mode is activated.*/
  bool verbose;

  /*! True if physics are enabled.*/
  bool enablePhysics;

  /*! True if artificial intelligence is activated.*/
  bool enableAI;

  /*! True if audio is enabled*/
  bool enableAudio;

  /*! True if high dynamic range rendering is enabled.*/
  bool enableHDR;

  /*! True if bloom is enabled.*/
  bool enableBloom;

  /*! True if shadows are enabled.*/
  bool enableShadows;

  /*! True if lights are enabled.*/
  bool enableLights;

  /*! True if particles are enabled.*/
  bool enableParticles;

  /*! True if decals are enabled.*/
  bool enableDecals;

  /*! True if sprites are enabled.*/
  bool enableSprites;

  /*! True if water is enabled.*/
  bool enableWater;

  /*! True if sky is enabled.*/
  bool enableSky;

  /*! True if camera collision is disabled.*/
  bool freeCameraDebug;

  /*! True if display mode is wireframe.*/
  bool wireframeDebug;

  /*! True if shadows are enabled.*/
  bool freezeCollisionDebug;

  /*! True if collision debug is visible.*/
  bool showCollisionDebug;

  /*! True if shadow debug is visible.*/
  bool showShadowDebug;

  /** Gravity factor.*/
  float gravity;

private:
  DataManager* dataManager;

  Ogre::String serialiseMoonPhase(int moonPhase);
  int calculateMoonPhase(Date date);
  Ogre::String day(Date date);
  Ogre::String month(Date date);
  Ogre::String serialiseDate(Date date);

};

