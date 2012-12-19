#pragma once

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <Game.h>

class NH2012 : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    NH2012(void);
    virtual ~NH2012(void);
    bool go(void);
protected:
    Game* game;

    Ogre::Root *root;
    Ogre::RenderWindow* window;

    //Input devices
    OIS::InputManager* inputManager;
    OIS::Mouse* mouse;
    OIS::Keyboard* keyboard;

    //Render events
    //void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    //void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

    //Window events
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

