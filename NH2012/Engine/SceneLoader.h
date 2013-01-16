#pragma once

#include <string>

#include "Vector3.h"
#include "Quaternion.h"

//need to include this directory
#include "../../RapidXml/rapidxml-1.13/rapidxml.hpp"

class Scene;

class SceneLoader
{
public:
  SceneLoader(void);
  virtual ~SceneLoader(void);

  void load(std::string filename, Scene* scene);//loads the specified file into the specified scene

private:
  Vector3 getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third);
  Quaternion getXMLRotation(rapidxml::xml_node<>* node);
  Ogre::ColourValue getXMLColour(rapidxml::xml_node<>* node, std::string first = "cr", std::string second = "cg", std::string third = "cb", std::string fourth = "ca");
  Vector3 getXMLScale(rapidxml::xml_node<>* node);
  Vector3 getXMLPosition(rapidxml::xml_node<>* node);
};

