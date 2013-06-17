#pragma once

//Standard
#include <map>
#include <string>
#include <vector>

//Boost
#include <boost/algorithm/string.hpp>

//Ogre
#include <OgreColourValue.h>

//Local
#include "Vector3.h"
#include "Container.h"

#define GRAVITY 0.0f, -9.81f, 0.0f

struct ArchitectureDesc
{
  ArchitectureDesc(int id, std::string name, std::string mesh)
    : name(name),
      mesh(mesh),
      id(id),
      friction(0.2f),
      restitution(0.5f),
      isNavigable(true),
      isPhysical(true),
      isStatic(true)
  {
  }

  std::string mesh;
  std::string name;

  float friction;
  float restitution;

  bool isNavigable;//enable ai navigation
  bool isStatic;//enable static mesh generation
  bool isPhysical;//enable physics fabrication

  int id;
};

struct ItemDesc
{
  ItemDesc(int id, std::string name, std::string mesh, std::string simplifiedMesh)
    : name(name),
      mesh(mesh),
      simplifiedMesh(simplifiedMesh),
      dynamicFriction(0.7f),
      staticFriction(0.7f),
      restitution(0.5f),
      density(0.1f),
      id(id),
      isDynamic(true),
      isPhysical(true)
  {
  }

  std::string mesh;
  std::string simplifiedMesh;//mesh suitable for collison
  std::string name;

  float dynamicFriction;
  float staticFriction;
  float restitution;
  float density;

  bool isDynamic;//movement applies eg. gravity and forces
  bool isPhysical;//can interact with the world

  int id;
};

struct CreatureDesc
{
  CreatureDesc(int id, std::string name, std::string mesh)
    : name(name),
      mesh(mesh),
      height(1.75f),
      speed(1.0f),
      health(100.0f),
      id(id),
      gravity(0,0,0)//GRAVITY)//temp disable gravity while fixing artificial intelligence
  {
  }

  std::string mesh;
  std::string name;

  float speed;
  float height;
  float health;
  Vector3 gravity;

  int id;
};

struct SceneDesc
{
  SceneDesc(int id, std::string name, std::string file)
    : name(name),
      file(file),
      id(id),
      ambientLight(0.1f,0.1f,0.1f),
      shadowColour(1.0f,1.0f,1.0f),
      gravity(GRAVITY),
      north(0.0f, 0.0f, 1.0f),
      fogStart(100.0f),
      fogEnd(200.0f),
      fogColour(0.5f,0.5f,0.5f)
  {
  }

  std::string file;
  std::string name;

  int id;

  Ogre::ColourValue ambientLight;
  Ogre::ColourValue shadowColour;
  Vector3 gravity;
  Vector3 north;
  float fogStart;
  float fogEnd;
  Ogre::ColourValue fogColour;
};

struct SoundDesc
{
  SoundDesc(int id, std::string name, std::string file)
    : name(name),
      file(file),
      id(id)
  {
  }

  std::string file;
  std::string name;

  int id;
};

struct PlayerDesc
{
  PlayerDesc()
    : mesh("actor.mesh"),
      name("Default Name"),
      gravity(GRAVITY)
  {
  }

  std::string mesh;
  std::string name;

  Vector3 gravity;
};

struct EmitterDesc
{
  EmitterDesc(int id, std::string name, std::string file)
    : id(id),
      name(name),
      file(file)
  {
  }

  int id;
  std::string name;
  std::string file;
};

struct LightDesc
{
  LightDesc(int id, std::string name, std::string file)
    : id(id),
      name(name),
      file(file)
  {
  }

  int id;
  std::string name;
  std::string file;
  float range;
  bool castShadows;
};

struct SpriteDesc
{
  SpriteDesc(int id, std::string name, std::string file)
    : id(id),
      name(name),
      file(file)
  {
  }

  int id;
  std::string name;
  std::string file;
};

struct AffectorDesc
{
  AffectorDesc(int id, std::string name, std::string script)
    : id(id),
      name(name),
      script(script)
  {
  }

  int id;
  std::string name;
  std::string script;
};

struct PortalDesc
{
  PortalDesc(int id, std::string name, std::string file)
    : id(id),
      name(name),
      file(file)
  {
  }

  int id;
  std::string name;
  std::string file;
};

struct InteractiveDesc
{
  InteractiveDesc(int id, std::string name, std::string script)
    : id(id),
      name(name),
      script(script)
  {
  }

  int id;
  std::string name;
  std::string script;
};

static const char SCENES_IDENTIFIER[] = "#Scenes";

static const char ARCHITECTURE_IDENTIFIER[] = "#Architecture";
static const char INTERACTIVES_IDENTIFIER[] = "#Interactives";
static const char CREATURES_IDENTIFIER[] = "#Creatures";
static const char ITEMS_IDENTIFIER[] = "#Items";
static const char PLAYERS_IDENTIFIER[] = "#Players";
static const char PORTALS_IDENTIFIER[] = "#Portals";

static const char SOUNDS_IDENTIFIER[] = "#Sounds";
static const char EMITTERS_IDENTIFIER[] = "#Emitters";
static const char LIGHTS_IDENTIFIER[] = "#Lights";
static const char SPRITES_IDENTIFIER[] = "#Sprites";

static const int ID_INDEX = 0;
static const int NAME_INDEX = 1;
static const int MESH_INDEX = 2;

/**
 * Handles the loading of all object information from the data files.
 */
class DataManager
{
public:
  DataManager(void);
  ~DataManager(void);

  void addData(std::string file);

  template<class Desc>
  Desc get(int id)
  {
    Container<Desc>* container = getContainer<Desc>();
    if(!container) throw NHException("the specified description type does not exist");
    Desc* desc = container->get(Id<Desc>(id));
    if(!desc) throw NHException("the specified description could not be found");
    return *desc;
  }

  std::vector<std::string> getLoadedDataFiles();

private:
  std::vector<std::string> files;

  Container<ArchitectureDesc> architecture;
  Container<CreatureDesc> creatures;
  Container<AffectorDesc> affectors;
  Container<EmitterDesc> emitters;
  Container<SpriteDesc> sprites;
  Container<PlayerDesc> players;
  Container<SceneDesc> scenes;
  Container<SoundDesc> sounds;
  Container<LightDesc> lights;
  Container<ItemDesc> items;

  template<class Desc>
  Container<Desc>* getContainer()
  {
    if(architecture.isType<Desc>()) return (Container<Desc>*)&architecture;
    if(creatures.isType<Desc>())    return (Container<Desc>*)&creatures;
    if(affectors.isType<Desc>())    return (Container<Desc>*)&affectors;
    if(emitters.isType<Desc>())     return (Container<Desc>*)&emitters;
    if(sprites.isType<Desc>())      return (Container<Desc>*)&sprites;
    if(players.isType<Desc>())      return (Container<Desc>*)&players;
    if(scenes.isType<Desc>())       return (Container<Desc>*)&scenes;
    if(sounds.isType<Desc>())       return (Container<Desc>*)&sounds;
    if(lights.isType<Desc>())       return (Container<Desc>*)&lights;
    if(items.isType<Desc>())        return (Container<Desc>*)&items;
    return NULL;
  }
};

