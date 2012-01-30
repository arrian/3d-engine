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

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
//#include <SdkCameraMan.h>

#include "../Game.h"
#include "../Singleplayer.h"
#include "../Multiplayer.h"

class NH2012 : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    NH2012(void);
    virtual ~NH2012(void);
    bool go(void);
protected:
    Ogre::Root *mRoot;
    //Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    Game* game;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    //OgreBites::SdkCameraMan* mCameraMan;     // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;   // sample details panel
    bool mCursorWasVisible;                  // was cursor visible before dialog appeared
    bool mShutDown;

    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    //void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    //void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif // #ifndef __NH2012_h_
