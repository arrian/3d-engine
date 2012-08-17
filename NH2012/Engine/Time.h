#pragma once

//Standard
#include <vector>
#include <ctime>
#include <iostream>
#include <string>

//Boost
#include <boost/lexical_cast.hpp>

//Ogre
#include <OgreString.h>

//Date Storage
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

class Time
{
public:
  Time(void);
  virtual ~Time(void);

  Date getDate();
  Ogre::String getDateString();
  Ogre::String getMoonPhase();
  bool isFriday13();
  bool isBirthday();
  bool isHalloween();
  bool isNewYears();

private:
  Ogre::String serialiseMoonPhase(int moonPhase);
  int calculateMoonPhase(Date date);
  Ogre::String day(Date date);
  Ogre::String month(Date date);
  Ogre::String serialiseDate(Date date);
};

