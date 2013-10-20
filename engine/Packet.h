#pragma once


#include "Vector3.h"
#include "Quaternion.h"
#include "Id.h"
#include "Endpoint.h"

class Player;
class Creature;
class Item;
class Scene;

#define MAX_PACKET_LENGTH 1024

enum PacketType
{
  WORLD_UPDATE,//world info and loaded scene info
  PLAYER_JOIN,//player joined the game
  PLAYER_LEFT,
  PLAYER_UPDATE,
  PLAYER_DEATH,
  ITEM_CREATE,
  ITEM_REMOVE,
  ITEM_UPDATE,
  CREATURE_CREATE,
  CREATURE_REMOVE,
  CREATURE_UPDATE,
  ACKNOWLEDGE,//acknowledge a create or remove event
  PING//maintain connection or get round trip time
};

struct PacketHeader
{
  unsigned long packetId;
  unsigned long timestamp;
  PacketType type;
  Id<Endpoint> sourceId;
};

struct PlayerPacket : public PacketHeader
{
  Id<Player> playerId;
  Id<Scene> sceneId;
  Vector3 position;
  Vector3 velocity;
  Vector3 lookAt;
  bool running;
  bool crouching;
};

struct ItemPacket : public PacketHeader
{
  Id<Item> itemId;
  Id<Scene> sceneId;
  Vector3 position;
  Quaternion rotation;
};

struct CreaturePacket : public PacketHeader
{
  Id<Creature> creatureId;
  Id<Scene> sceneId;
  Vector3 position;
};


class Packet
{
public:
  Packet(void);
  virtual ~Packet(void);

  int getDataLength() { return MAX_PACKET_LENGTH; }

  PacketHeader* getHeader() {return getDataPointer<PacketHeader>();}

  template<class T>
  T* getDataPointer()
  {
    return reinterpret_cast<T*>(&data);
  }
  
  char data[MAX_PACKET_LENGTH];

};
