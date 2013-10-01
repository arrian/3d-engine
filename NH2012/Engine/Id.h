
#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <boost/lexical_cast.hpp>


template<class T>
class Id
{
public:

  typedef T type;

  //Generate an id
  Id()
  {
    globalInstance++;
    this->instance = globalInstance;
  }

  //Initialise specifying the id
  Id(long instance)
  {
    if(instance > globalInstance) globalInstance = instance + 1;
    this->instance = instance;
  }

  long getInstance() const
  {
    return instance;
  }

  bool isValid()
  {
    if(instance == 0) return false;
    return true;
  }

  //Gets the identifier name in the format "Object::Instance"
  std::string getName()
  {
    std::stringstream stream;
    stream << std::setw(8) << std::setfill('0') << std::hex << instance;
    return std::string(typeid(T).name()) + "::" + stream.str();
  }

  inline bool operator==(const Id<T>& other) const 
  {
    return getInstance() == other.getInstance();
  }

  inline bool operator!=(const Id<T>& other) const 
  {
    return !(*this == other);
  }

  inline bool operator<( const Id<T>& other) const
  {
    return getInstance() < other.getInstance();
  }

private:
  static long globalInstance;
  long instance;

};


//Intialising the global instance number.
template <typename T>
long Id<T>::globalInstance = 1;//saving zero for invalid id

