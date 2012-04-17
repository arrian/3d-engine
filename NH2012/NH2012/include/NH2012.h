/*
-----------------------------------------------------------------------------
Filename:    NH2012.h
-----------------------------------------------------------------------------
*/
#ifndef __NH2012_h_
#define __NH2012_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

//#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <Game.h>
#include <Singleplayer.h>
#include <Multiplayer.h>

class NH2012 : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
    NH2012(void);
    virtual ~NH2012(void);
    bool go(void);
protected:
    Game* game;

    Ogre::Root *root;
    //Ogre::SceneManager* sceneManager;
    Ogre::RenderWindow* window;
    Ogre::String resources;
    Ogre::String plugins;

    bool cursorWasVisible;
    bool shutDown;

    //Input devices
    OIS::InputManager* inputManager;
    OIS::Mouse* mouse;
    OIS::Keyboard* keyboard;

    //Input events
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    //Render events
    //void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    //void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

    //Window events
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif // #ifndef __NH2012_h_
