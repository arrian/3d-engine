#include "ControlManager.h"

ControlManager::ControlManager()
{
  stringToEnum = boost::assign::map_list_of("W",OIS::KC_W)("A",OIS::KC_A)("S",OIS::KC_S)("D",OIS::KC_D)
    ("TAB",OIS::KC_TAB)("LSHIFT",OIS::KC_LSHIFT)("GRAVE",OIS::KC_GRAVE)("F",OIS::KC_F)
    ("SPACE",OIS::KC_SPACE)("1",OIS::KC_1)("2",OIS::KC_2)
    ("LCONTROL",OIS::KC_LCONTROL)("ESCAPE",OIS::KC_ESCAPE);
}

//-------------------------------------------------------------------------------------
ControlManager::~ControlManager()
{

}

//-------------------------------------------------------------------------------------
OIS::KeyCode ControlManager::stringToKeyCode(std::string string)
{
  if(stringToEnum.count(string) < 1) throw NHException("Could not find a mapping for a string in ControlManager.");
  return stringToEnum.find(string)->second;
}