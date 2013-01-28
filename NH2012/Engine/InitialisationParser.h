#pragma once

#include <string>

//Boost
#include <boost/property_tree/ptree.hpp>


class World;

class InitialisationParser
{
public:
  InitialisationParser(std::string filename);
  virtual ~InitialisationParser(void);

  template<typename T> T get(std::string label);
 
private:
  boost::property_tree::ptree pt;

};

