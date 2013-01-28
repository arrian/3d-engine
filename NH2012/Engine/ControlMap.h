#pragma once

//Standard
#include <string>

//Boost
#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>

//OIS
#include "OIS.h"

//Local
#include "Button.h"
#include "NHException.h"


typedef boost::bimap<OIS::KeyCode, std::string> MapperKeyboard;
typedef boost::bimap<OIS::MouseButtonID, std::string> MapperMouse;

/**Maps OIS keys to strings and strings to keys*/
class ControlMap
{
public:
  static ControlMap& getInstance()
  {
    static ControlMap instance;
    return instance;
  }

  std::string getAsString(OIS::KeyCode key, bool capital)
  {
    if(mapperKeyboard.left.count(key) == 0) throw NHException("could not convert the given keycode to a string");
    std::string result = mapperKeyboard.left.find(key)->second;
    if(capital)
    {
      if(lowerToUpper.count(result) == 0)//probably a letter key
      {
        if(result.size() > 1) throw NHException("could not convert the given keycode to an uppercase string");//not a letter key
        boost::to_upper(result);
      }
      else result = lowerToUpper.find(result)->second;
    }
    return result;
  }

  std::string getAsString(OIS::MouseButtonID button)
  {
    if(mapperMouse.left.count(button) == 0) throw NHException("could not convert the given mouse button if to a string");
    return mapperMouse.left.find(button)->second;
  }

  bool isKeyboardButtonString(std::string string)
  {
    return mapperKeyboard.right.count(string) != 0;
  }

  bool isMouseButtonString(std::string string)
  {
    return mapperMouse.right.count(string) != 0;
  }

  OIS::KeyCode getAsKeyboardButton(std::string string)
  {
    if(!isKeyboardButtonString(string)) throw NHException("could not convert the given string to a keycode");
    return mapperKeyboard.right.find(string)->second;
  }

  OIS::MouseButtonID getAsMouseButton(std::string string)
  {
    if(!isMouseButtonString(string)) throw NHException("could not convert the given string to a mouse button id");
    return mapperMouse.right.find(string)->second;
  }

private:
  ControlMap()
  {
    mapperMouse = boost::assign::list_of<MapperMouse::relation>
                  (OIS::MB_Left,    "MouseButtonLeft")
                  (OIS::MB_Middle,  "MouseButtonMiddle")
                  (OIS::MB_Right,   "MouseButtonRight")
                  (OIS::MB_Button3, "MouseButton3")
                  (OIS::MB_Button4, "MouseButton4")
                  (OIS::MB_Button5, "MouseButton5")
                  (OIS::MB_Button6, "MouseButton6")
                  (OIS::MB_Button7, "MouseButton7");

    mapperKeyboard = boost::assign::list_of<MapperKeyboard::relation>
                     (OIS::KC_A, "a")(OIS::KC_1, "1")
                     (OIS::KC_B, "b")(OIS::KC_2, "2")
                     (OIS::KC_C, "c")(OIS::KC_3, "3")
                     (OIS::KC_D, "d")(OIS::KC_4, "4")
                     (OIS::KC_E, "e")(OIS::KC_5, "5")
                     (OIS::KC_F, "f")(OIS::KC_6, "6")
                     (OIS::KC_G, "g")(OIS::KC_7, "7")
                     (OIS::KC_H, "h")(OIS::KC_8, "8")
                     (OIS::KC_I, "i")(OIS::KC_9, "9")
                     (OIS::KC_J, "j")(OIS::KC_0, "0")
                     (OIS::KC_K, "k")(OIS::KC_GRAVE, "`")
                     (OIS::KC_L, "l")(OIS::KC_MINUS, "-")
                     (OIS::KC_M, "m")(OIS::KC_EQUALS, "=")
                     (OIS::KC_N, "n")(OIS::KC_LBRACKET, "[")
                     (OIS::KC_O, "o")(OIS::KC_RBRACKET, "]")
                     (OIS::KC_P, "p")(OIS::KC_BACKSLASH, "\\")
                     (OIS::KC_Q, "q")(OIS::KC_SEMICOLON, ";")
                     (OIS::KC_R, "r")(OIS::KC_APOSTROPHE, "'")
                     (OIS::KC_S, "s")(OIS::KC_COMMA, ",")
                     (OIS::KC_T, "t")(OIS::KC_PERIOD, ".")
                     (OIS::KC_U, "u")(OIS::KC_SLASH, "/")
                     (OIS::KC_V, "v")
                     (OIS::KC_W, "w")
                     (OIS::KC_X, "x")
                     (OIS::KC_Y, "y")
                     (OIS::KC_Z, "z")
                     (OIS::KC_SPACE, "Space")
                     (OIS::KC_LCONTROL, "ControlLeft")
                     (OIS::KC_RCONTROL, "ControlRight")
                     (OIS::KC_LSHIFT, "ShiftLeft")
                     (OIS::KC_RSHIFT, "ShiftRight")
                     (OIS::KC_LMENU, "AltLeft")
                     (OIS::KC_RMENU, "AltRight")
                     (OIS::KC_TAB, "Tab")
                     (OIS::KC_RETURN, "Enter")
                     (OIS::KC_BACK, "Backspace")
                     (OIS::KC_HOME, "Home")
                     (OIS::KC_END, "End")
                     (OIS::KC_PGUP, "PageUp")
                     (OIS::KC_PGDOWN, "PageDown")
                     (OIS::KC_INSERT, "Insert")
                     (OIS::KC_PAUSE, "Pause")
                     (OIS::KC_ESCAPE, "Escape")
                     (OIS::KC_F1, "F1")(OIS::KC_F2, "F2")(OIS::KC_F3, "F3")
                     (OIS::KC_F4, "F4")(OIS::KC_F5, "F5")(OIS::KC_F6, "F6")(OIS::KC_F7, "F7")
                     (OIS::KC_F8, "F8")(OIS::KC_F9, "F9")(OIS::KC_F10, "F10")
                     (OIS::KC_F11, "F11")(OIS::KC_F12, "F12")(OIS::KC_F13, "F13")
                     (OIS::KC_F14, "F14")(OIS::KC_F15, "F15");
             
    lowerToUpper = boost::assign::map_list_of<std::string, std::string>
                   ("`","~")("1","!")("2","@")("3","#")("4","$")("5","%")
                   ("6","^")("7","&")("8","*")("9","(")("0",")")("-","_")
                   ("=","+")("[","{")("]","}")("\\","|")(";",":")("'","\"")
                   (",","<")(".",">")("/","?");
  }

  ~ControlMap(void)
  {

  }

  ControlMap(ControlMap const&);    // Don't implement
  void operator=(ControlMap const&); // Don't implement

  MapperMouse mapperMouse;

  MapperKeyboard mapperKeyboard;
  std::map<std::string, std::string> lowerToUpper;
};