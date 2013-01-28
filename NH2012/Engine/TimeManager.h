#pragma once

//Standard
#include <ctime>
#include <iostream>
#include <string>


enum MoonPhase
{
  NEW,
  WAXING_CRESCENT,
  FIRST_QUARTER,
  WAXING_GIBBOUS,
  FULL,
  WANING_GIBBOUS,
  THIRD_QUARTER,
  WANING_CRESCENT
};

enum Day
{
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY
};

enum Month
{
  JANUARY,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER,
  DECEMBER
};

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



class TimeManager
{
public:
  TimeManager(void);
  virtual ~TimeManager(void);

  Date getDate();
  std::string getDateString();
  std::string getMoonPhase();
  bool isFriday13();
  bool isBirthday();//currently set to 30th October
  bool isHalloween();
  bool isNewYears();

  void update(double elapsedSeconds);
  unsigned long getTicks();

private:
  //Serialisers
  std::string serialiseDate(Date date);
  std::string serialiseMoonPhase(MoonPhase moonPhase);
  std::string serialiseDay(Day day);
  std::string serialiseMonth(Month month);

  unsigned long ticks;

  //Getters
  MoonPhase getMoonPhase(Date date);
  Day getDay(Date date);
  Month getMonth(Date date);
};

