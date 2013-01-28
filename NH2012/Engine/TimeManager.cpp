#include "TimeManager.h"

//Boost
#include <boost/lexical_cast.hpp>

//Local
#include "NHException.h"


TimeManager::TimeManager(void)
  : ticks(0)
{
}

//-------------------------------------------------------------------------------------
TimeManager::~TimeManager(void)
{
}

//-------------------------------------------------------------------------------------
void TimeManager::update(double elapsedSeconds)
{
  if(ticks < ULONG_MAX - 1) ticks++;
  else ticks = 0;
}

//-------------------------------------------------------------------------------------
unsigned long TimeManager::getTicks()
{
  return ticks;
}

//-------------------------------------------------------------------------------------
Date TimeManager::getDate()
{
  return Date();
}

//-------------------------------------------------------------------------------------
std::string TimeManager::getDateString()
{
  return serialiseDate(Date());
}

//-------------------------------------------------------------------------------------
std::string TimeManager::getMoonPhase()
{
  return serialiseMoonPhase(getMoonPhase(Date()));
}

//-------------------------------------------------------------------------------------
std::string TimeManager::serialiseDate(Date date)
{
  std::stringstream dateString;
  dateString << date.day << "/" << date.month << "/" << date.year;
  return (dateString.str());
}

//-------------------------------------------------------------------------------------
bool TimeManager::isFriday13()
{
  Date date = Date();
  return ((getDay(date) == FRIDAY) && (date.day == 13));
}

//-------------------------------------------------------------------------------------
bool TimeManager::isBirthday()
{
  Date date = Date();
  return (getMonth(date) == OCTOBER && date.day == 30);
}

//-------------------------------------------------------------------------------------
bool TimeManager::isHalloween()
{
  Date date = Date();
  return (getMonth(date) == OCTOBER && date.day == 31);
}

//-------------------------------------------------------------------------------------
bool TimeManager::isNewYears()
{
  Date date = Date();
  return (getMonth(date) == JANUARY && date.day == 1);
}


//-------------------------------------------------------------------------------------
MoonPhase TimeManager::getMoonPhase(Date date)
{
  //Calculates the moon phase (0-7).
  //0 => new moon.
  //4 => full moon.

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
  jd = c + e + date.day - 694039.09;//jd is total days elapsed
  jd /= 29.53;//divide by the moon cycle (29.53 days)
  b = boost::lexical_cast<int>(jd);//int(jd) -> b, take integer part of jd
  jd -= b;//subtract integer part to leave fractional part of original jd
  b = boost::lexical_cast<int>(jd * 8 + 0.5);//scale fraction from 0-8 and round by adding 0.5
  b = b & 7;// 0 and 8 are the same so turn 8 into 0
  
  MoonPhase phase[] = {NEW, WAXING_CRESCENT, FIRST_QUARTER, WAXING_GIBBOUS, FULL, WANING_GIBBOUS, THIRD_QUARTER, WANING_CRESCENT};

  if(b < 0) throw NHException("Get requested for unknown moon phase.");
  return phase[b];
}

//-------------------------------------------------------------------------------------
Day TimeManager::getDay(Date date)
{
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  date.year -= date.month < 3;
  int numerical = (date.year + date.year / 4 - date.year / 100 + date.year / 400 + t[date.month - 1] + date.day) % 7;

  Day days[] = {MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY};

  if(numerical > 6 || numerical <= 0) throw NHException("Get requested for unknown day of the week.");
  return days[numerical - 1];
}

//-------------------------------------------------------------------------------------
Month TimeManager::getMonth(Date date)
{
  Month months[] = {JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER};

  if(date.month <= 0 || date.month > 11) throw NHException("Get requested for unknown month.");
  return months[date.month - 1];
}

//-------------------------------------------------------------------------------------
std::string TimeManager::serialiseMoonPhase(MoonPhase moonPhase)
{
  switch(moonPhase)
  {
    case NEW:             return "new moon";
    case WAXING_CRESCENT: return "waxing crescent moon";
    case FIRST_QUARTER:   return "first quarter moon";
    case WAXING_GIBBOUS:  return "waxing gibbous moon";
    case FULL:            return "full moon";
    case WANING_GIBBOUS:  return "waning gibbous moon";
    case THIRD_QUARTER:   return "third quarter moon";
    case WANING_CRESCENT: return "waning crescent moon";
    default:              throw NHException("Serialise requested for unknown moon phase.");
  }
}

//-------------------------------------------------------------------------------------
std::string TimeManager::serialiseDay(Day day)
{
  switch(day)
  {
    case MONDAY:    return "monday";
    case TUESDAY:   return "tuesday";
    case WEDNESDAY: return "wednesday";
    case THURSDAY:  return "thursday";
    case FRIDAY:    return "friday";
    case SATURDAY:  return "saturday";
    case SUNDAY:    return "sunday";
    default:        throw NHException("Serialise requested for unknown day of the week.");
  }
}

//-------------------------------------------------------------------------------------
std::string TimeManager::serialiseMonth(Month month)
{
  switch(month)
  {
    case JANUARY:   return "january";
    case FEBRUARY:  return "february";
    case MARCH:     return "march";
    case APRIL:     return "april";
    case MAY:       return "may";
    case JUNE:      return "june";
    case JULY:      return "july";
    case AUGUST:    return "august";
    case SEPTEMBER: return "september";
    case OCTOBER:   return "october";
    case NOVEMBER:  return "november";
    case DECEMBER:  return "december";
    default:        throw NHException("Serialise requested for unknown month.");
  }
}


