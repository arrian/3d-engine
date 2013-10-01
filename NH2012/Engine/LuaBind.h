
#include <iostream>

#include "ScriptManager.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "TimeManager.h"


extern "C"
{
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

#include <luabind/luabind.hpp>

extern "C" int bindLua(lua_State* L)
{
  using namespace luabind;

  open(L);

  module(L)
    [
      class_<World>("World")
      .def("getScene", &World::getScene)
      .def("getScriptManager", &World::getScriptManager)
      .def("getDataManager", &World::getDataManager)
      .def("getGraphicsManager", &World::getGraphicsManager)
      .def("getPhysicsManager", &World::getPhysicsManager)
      .def("getSoundManager", &World::getSoundManager)
      .def("getTimeManager", &World::getTimeManager),

      class_<ScriptManager>("ScriptManager")
      .def("display", (void(ScriptManager::*)(std::string))&ScriptManager::display)
      .def("run", &ScriptManager::run),

      class_<DataManager>("DataManager")
      .def("addData", &DataManager::addData)
      .def("getItemDesc", &DataManager::get<ItemDesc>)
      .def("getArchitectureDesc", &DataManager::get<ArchitectureDesc>)
      .def("getCreatureDesc", &DataManager::get<CreatureDesc>)
      .def("getSceneDesc", &DataManager::get<SceneDesc>),

      class_<GraphicsManager>("GraphicsManager")
      .def("isBloomEnabled", &GraphicsManager::isBloomEnabled),

      class_<PhysicsManager>("PhysicsManager")
      .def("setEnabled", &PhysicsManager::setEnabled),

      class_<SoundManager>("SoundManager")
      .def("setEnabled", &SoundManager::setEnabled),

      class_<TimeManager>("TimeManager")
      .def("getTicks", &TimeManager::getTicks),

      class_<ControlManager>("ControlManager"),

      class_<NetworkManager>("NetworkManager")
      .def("connect", &NetworkManager::connect)

    ];

  return 0;
}


