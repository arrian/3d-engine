#pragma once

/**
 * Wrapper for an integer priority.
 * Values range from 0 (Low) to 100 (High).
 **/
class Priority
{
public:
  int level;

  Priority(int level)
    : level(level)
  {
  }

  static Priority LOW;
  static Priority MEDIUM;
  static Priority HIGH;

  inline friend bool operator==(const Priority& lhs, const Priority& rhs){return lhs.level == rhs.level;} 
  inline friend bool operator!=(const Priority& lhs, const Priority& rhs){return !operator==(lhs,rhs);} 
  inline friend bool operator< (const Priority& lhs, const Priority& rhs){return lhs.level < rhs.level;} 
  inline friend bool operator> (const Priority& lhs, const Priority& rhs){return  operator< (rhs,lhs);} 
  inline friend bool operator<=(const Priority& lhs, const Priority& rhs){return !operator> (lhs,rhs);} 
  inline friend bool operator>=(const Priority& lhs, const Priority& rhs){return !operator< (lhs,rhs);}
};



