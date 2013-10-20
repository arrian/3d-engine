#pragma once

#include "../../Engine/NHException.h"
#include "qnamespace.h"
#include "OISKeyboard.h"
#include "boost/assign.hpp"
#include <map>

class QtOISConversion
{
public:
  std::map<int, OIS::KeyCode> conversion;

  QtOISConversion()
  {
    conversion = boost::assign::map_list_of 
                 (Qt::Key_0, OIS::KC_0)
                 (Qt::Key_1, OIS::KC_1)
                 (Qt::Key_2, OIS::KC_2)
                 (Qt::Key_4, OIS::KC_4)
                 (Qt::Key_5, OIS::KC_5)
                 (Qt::Key_6, OIS::KC_6)
                 (Qt::Key_7, OIS::KC_7)
                 (Qt::Key_8, OIS::KC_8)
                 (Qt::Key_9, OIS::KC_9)
                 (Qt::Key_A, OIS::KC_A)
                 (Qt::Key_B, OIS::KC_B)
                 (Qt::Key_C, OIS::KC_C)
                 (Qt::Key_D, OIS::KC_D)
                 (Qt::Key_E, OIS::KC_E)
                 (Qt::Key_F, OIS::KC_F)
                 (Qt::Key_G, OIS::KC_G)
                 (Qt::Key_H, OIS::KC_H)
                 (Qt::Key_I, OIS::KC_I)
                 (Qt::Key_J, OIS::KC_J)
                 (Qt::Key_K, OIS::KC_K)
                 (Qt::Key_L, OIS::KC_L)
                 (Qt::Key_M, OIS::KC_M)
                 (Qt::Key_N, OIS::KC_N)
                 (Qt::Key_O, OIS::KC_O)
                 (Qt::Key_P, OIS::KC_P)
                 (Qt::Key_Q, OIS::KC_Q)
                 (Qt::Key_R, OIS::KC_R)
                 (Qt::Key_S, OIS::KC_S)
                 (Qt::Key_T, OIS::KC_T)
                 (Qt::Key_U, OIS::KC_U)
                 (Qt::Key_V, OIS::KC_V)
                 (Qt::Key_W, OIS::KC_W)
                 (Qt::Key_X, OIS::KC_X)
                 (Qt::Key_Y, OIS::KC_Y)
                 (Qt::Key_Z, OIS::KC_Z)
                 (Qt::Key_Space, OIS::KC_SPACE)
                 (Qt::Key_Shift, OIS::KC_LSHIFT)
                 (Qt::Key_Control, OIS::KC_LCONTROL)
                 (Qt::Key_QuoteLeft, OIS::KC_GRAVE)
                 ;
  }

  OIS::KeyCode get(int key)
  {
    if(conversion.count(key) == 0) throw NHException("qt to ois key conversion was not found");
    return conversion.find(key)->second;
  }
};