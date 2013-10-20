#include "WorldLoader.h"

#include <boost/assign/list_of.hpp>

#include "World.h"
#include "InitialisationParser.h"

void WorldLoader::load(std::string filename, World* world)
{
  InitialisationParser ini(filename);

  //General
  world->physicsManager.setEnabled(ini.get<bool>("General.EnablePhysics"));
  world->soundManager.setEnabled(ini.get<bool>("General.EnableAudio"));
  //enableAI = ini.get<bool>("General.EnableAI");

  //Controls
  world->controlManager.moveForward = Button(ini.get<std::string>("Controls.Forward"));
  world->controlManager.moveLeft = Button(ini.get<std::string>("Controls.Left"));
  world->controlManager.moveBack = Button(ini.get<std::string>("Controls.Back"));
  world->controlManager.moveRight = Button(ini.get<std::string>("Controls.Right"));
  world->controlManager.jump = Button(ini.get<std::string>("Controls.Jump"));
  world->controlManager.run = Button(ini.get<std::string>("Controls.Run"));
  world->controlManager.crouch = Button(ini.get<std::string>("Controls.Crouch"));
  world->controlManager.interact = Button(ini.get<std::string>("Controls.Interact"));
  for(int i = 1; i < 10; i++) world->controlManager.quickslots.push_back(Button(ini.get<std::string>("Controls.QuickSlot" + boost::lexical_cast<std::string>(i))));
  world->controlManager.console = Button(ini.get<std::string>("Controls.Console"));
  world->controlManager.exit = Button(ini.get<std::string>("Controls.Exit"));
  world->controlManager.leftHand = Button(ini.get<std::string>("Controls.LeftHand"));//OIS::MB_Left;
  world->controlManager.rightHand = Button(ini.get<std::string>("Controls.RightHand"));//OIS::MB_Right;

  //Temp Controls
  world->controlManager.addItem = OIS::KC_1;
  world->controlManager.addCreature = OIS::KC_2;
  
  //Environment
  //world->graphicsManager.setShadowsEnabled(ini.get<bool>("Environment.Shadows"));
  //world->graphicsManager.setBloomEnabled(ini.get<bool>("Environment.Bloom"));
  //world->graphicsManager.setSSAOEnabled(ini.get<bool>("Environment.SSAO"));
  
  /*
  world->enableHDR = ini.get<bool>("Environment.HDR");
  world->enableMotionBlur = ini.get<bool>("Environment.MotionBlur");
  world->enableLights = ini.get<bool>("Environment.Lights");
  world->enableParticles = ini.get<bool>("Environment.Particles");
  world->enableDecals = ini.get<bool>("Environment.Decals");
  world->enableSprites = ini.get<bool>("Environment.Sprites");
  world->enableWater = ini.get<bool>("Environment.Water");
  world->enableSky = ini.get<bool>("Environment.Sky");
  */

  //Data
  std::vector<std::string> data = boost::assign::list_of("Data.Scenes")("Data.Architecture")("Data.Creatures")("Data.Items")("Data.Sounds");
  for(std::vector<std::string>::iterator it = data.begin(); it != data.end(); ++it) world->dataManager.addData(ini.get<std::string>(*it));

  //Debug
  /*
  world->freezeCollisionDebug = ini.get<bool>("Debug.FreezeCollision");
  world->freeCameraDebug = ini.get<bool>("Debug.FreeCamera");
  world->wireframeDebug = ini.get<bool>("Debug.Wireframe");
  world->showCollisionDebug = ini.get<bool>("Debug.ShowCollisionsDebug");
  world->showShadowDebug = ini.get<bool>("Debug.ShowShadowDebug");
  */
  world->defaultScene = Id<Scene>(ini.get<int>("Debug.DefaultScene"));

#ifdef _DEBUG
  std::string physXVisualDebuggerIP = ini.get<std::string>("Debug.PhysXVisualDebuggerIP");
  int physXVisualDebuggerPort = ini.get<int>("Debug.PhysXVisualDebuggerPort");
  int physXVisualDebuggerTimeoutMilliseconds = ini.get<int>("Debug.PhysXVisualDebuggerTimeoutMilliseconds");

  world->physicsManager.initialiseVisualDebugger(physXVisualDebuggerIP, physXVisualDebuggerPort, physXVisualDebuggerTimeoutMilliseconds);
#endif
}
