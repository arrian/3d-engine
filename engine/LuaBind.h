
#include <iostream>

#include "ScriptManager.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "TimeManager.h"

#include <stdio.h>

#include <luabind/luabind.hpp>

extern "C"
{
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

extern "C" int printLua(lua_State* state)
{
  int nargs = lua_gettop(state);
  for(int i = 1; i <= nargs; ++i) 
  {
    lua_getglobal(state, "script");
    lua_gettop(state);
    int len = _snprintf(NULL, 0, "%s%s%s", "script:display(\"", lua_tostring(state, i), "\")");

    char* text = (char*) malloc(len + 1);

    sprintf(text, "%s%s%s", "script:display(\"", lua_tostring(state, i), "\")");
    luaL_dostring(state, text);

    free(text);
  }
  printf("\n");
  return 0;
}

extern "C" int bindLua(lua_State* L)
{
  using namespace luabind;

  open(L);

  module(L)
    [
      class_<Item         >("Item"),
      class_<Effect       >("Effect"),
      class_<Portal       >("Portal"),
      class_<Creature     >("Creature"),
      class_<Interactive  >("Interactive"),
      class_<Architecture >("Architecture"),

      class_<Id<Item>         >("ItemId"),
      class_<Id<Effect>       >("EffectId"),
      class_<Id<Portal>       >("PortalId"),
      class_<Id<Player>       >("PlayerId"),
      class_<Id<Creature>     >("CreatureId"),
      class_<Id<Interactive>  >("InteractiveId"),
      class_<Id<Architecture> >("ArchitectureId"),

      class_<ItemDesc>("ItemDesc"),
      class_<PlayerDesc>("PlayerDesc"),
      class_<ArchitectureDesc>("ArchitectureDesc"),
      class_<CreatureDesc>("CreatureDesc"),
      class_<SceneDesc>("SceneDesc"),
      class_<SoundDesc>("SoundDesc"),
      class_<EmitterDesc>("EmitterDesc"),
      class_<LightDesc>("LightDesc"),
      class_<PortalDesc>("PortalDesc"),
      class_<InteractiveDesc>("InteractiveDesc"),

      class_<World>("World")
      .def("getScene",           &World::getScene)
      .def("getScriptManager",   &World::getScriptManager)
      .def("getDataManager",     &World::getDataManager)
      .def("getGraphicsManager", &World::getGraphicsManager)
      .def("getPhysicsManager",  &World::getPhysicsManager)
      .def("getSoundManager",    &World::getSoundManager)
      .def("getTimeManager",     &World::getTimeManager)
      .def("getSceneCount",      &World::getSceneCount)
      .def("getPlayer",          &World::getPlayer)
      .def("loadPlayer",         &World::loadPlayer)
      .def("loadScene",          &World::loadScene),

      class_<ScriptManager>("ScriptManager")
      .def("display", (void(ScriptManager::*)(std::string))&ScriptManager::display)
      .def("runScript", &ScriptManager::runScript)
      .def("runString", &ScriptManager::runString),

      class_<DataManager>("DataManager")
      .def("addData",             &DataManager::addData)
      .def("getItemDesc",         &DataManager::get<ItemDesc>)
      .def("getArchitectureDesc", &DataManager::get<ArchitectureDesc>)
      .def("getCreatureDesc",     &DataManager::get<CreatureDesc>)
      .def("getSceneDesc",        &DataManager::get<SceneDesc>)
      .def("getLoadedDataFiles",  &DataManager::getLoadedDataFiles),

      class_<GraphicsManager>("GraphicsManager")
      .def("isBloomEnabled",    &GraphicsManager::isBloomEnabled)
      .def("isShadowsEnabled",  &GraphicsManager::isShadowsEnabled)
      .def("isSSAOEnabled",     &GraphicsManager::isSSAOEnabled)
      .def("setBloomEnabled",   &GraphicsManager::setBloomEnabled)
      .def("setShadowsEnabled", &GraphicsManager::setShadowsEnabled)
      .def("setSSAOEnabled",    &GraphicsManager::setSSAOEnabled),

      class_<PhysicsManager>("PhysicsManager")
      .def("setEnabled", &PhysicsManager::setEnabled)
      .def("isEnabled",  &PhysicsManager::isEnabled),

      class_<SoundManager>("SoundManager")
      .def("setEnabled", &SoundManager::setEnabled)
      .def("setMuteAll", &SoundManager::setMuteAll),

      class_<TimeManager>("TimeManager")
      .def("getTicks", &TimeManager::getTicks)
      .def("getDateString",&TimeManager::getDateString)
      .def("get", &TimeManager::getMoonPhaseString),

      class_<ControlManager>("ControlManager"),

      class_<NetworkManager>("NetworkManager")
      .def("connect", &NetworkManager::connect),

      class_<Scene>("Scene")
      .def("reset", &Scene::reset)
      .def("getId", &Scene::getId)
      .def("getName", &Scene::getName)
      .def("addItem",         &Scene::addItem)
      .def("addArchitecture", &Scene::addArchitecture)
      .def("addCreature",     &Scene::addCreature)
      .def("addInteractive",  &Scene::addInteractive)
      .def("addLight",        &Scene::addLight)
      .def("addPlayer",       &Scene::addPlayer)
      .def("addPortal",       &Scene::addPortal)
      .def("getItem",         &Scene::get<Item>)
      .def("getArchitecture", &Scene::get<Architecture>)
      .def("getCreature",     &Scene::get<Creature>)
      .def("getInteractive",  &Scene::get<Interactive>)
      .def("getLight",        &Scene::get<Light>)
      .def("getPlayer",       &Scene::get<Player>)
      .def("getPortal",       &Scene::get<Portal>)
      .def("removeItem",         (boost::shared_ptr<Item>(Scene::*)(Id<Item>))&Scene::remove<Item>)
      .def("removeArchitecture", (boost::shared_ptr<Architecture>(Scene::*)(Id<Architecture>))&Scene::remove<Architecture>)
      .def("removeCreature",     (boost::shared_ptr<Creature>(Scene::*)(Id<Creature>))&Scene::remove<Creature>)
      .def("removeInteractive",  (boost::shared_ptr<Interactive>(Scene::*)(Id<Interactive>))&Scene::remove<Interactive>)
      .def("removeLight",        (boost::shared_ptr<Light>(Scene::*)(Id<Light>))&Scene::remove<Light>)
      .def("removePlayer",       (boost::shared_ptr<Player>(Scene::*)(Id<Player>))&Scene::remove<Player>)
      .def("removePortal",       (boost::shared_ptr<Portal>(Scene::*)(Id<Portal>))&Scene::remove<Portal>)
      .def("containsItem",         &Scene::contains<Item>)
      .def("containsArchitecture", &Scene::contains<Architecture>)
      .def("containsCreature",     &Scene::contains<Creature>)
      .def("containsInteractive",  &Scene::contains<Interactive>)
      .def("containsLight",        &Scene::contains<Light>)
      .def("containsPlayer",       &Scene::contains<Player>)
      .def("containsPortal",       &Scene::contains<Portal>)
      .def("countItem",         &Scene::count<Item>)
      .def("countArchitecture", &Scene::count<Architecture>)
      .def("countCreature",     &Scene::count<Creature>)
      .def("countInteractive",  &Scene::count<Interactive>)
      .def("countLight",        &Scene::count<Light>)
      .def("countPlayer",       &Scene::count<Player>)
      .def("countPortal",       &Scene::count<Portal>)
      .def("getScenePhysicsManager",      &Scene::getScenePhysicsManager)
      .def("getSceneGraphicsManager",     &Scene::getSceneGraphicsManager)
      .def("getScenePathfindManager",     &Scene::getScenePathfindManager),

      class_<ScenePhysicsManager>("ScenePhysicsManager")
      .def("getGravity", &ScenePhysicsManager::getGravity)
      .def("setGravity", &ScenePhysicsManager::setGravity),

      class_<SceneGraphicsManager>("SceneGraphicsManager")
      .def("setAmbientLight", &SceneGraphicsManager::setAmbientLight)
      .def("setShadowsEnabled",&SceneGraphicsManager::setShadowsEnabled)
      .def("setDebugDrawShadows",&SceneGraphicsManager::setDebugDrawShadows)
      .def("setDebugDrawBoundingBoxes",&SceneGraphicsManager::setDebugDrawBoundingBoxes),

      class_<ScenePathfindManager>("ScenePathfindManager")
      .def("getClosestNavigablePoint",&ScenePathfindManager::getClosestNavigablePoint)
      .def("getRandomNavigablePoint",&ScenePathfindManager::getRandomNavigablePoint)
      .def("getRandomNavigablePointInCircle",&ScenePathfindManager::getRandomNavigablePointInCircle)
      .def("setDrawNavigationMesh",&ScenePathfindManager::setDrawNavigationMesh)
      .def("isReachable",&ScenePathfindManager::isReachable),

      //class_<SceneArchitectureManager>("SceneArchitectureManager"),

      class_<Player>("Player")
      .def("setPosition", &Player::setPosition)
      .def("getPosition", &Player::getPosition),

      class_<ConsoleScreen>("Console")
      .def("clear", &ConsoleScreen::clear),

      class_<Vector3>("Vector3"),
      class_<Quaternion>("Quaternion")




    ];

  return 0;
}


