#pragma once


#include "Vector3.h"

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
  MONSTER_CREATE,
  MONSTER_REMOVE,
  MONSTER_UPDATE,
  ACKNOWLEDGE,//acknowledge a create or remove event
  PING//maintain connection or get round trip time
};

struct PacketHeader
{
  PacketType type;
  int id;//the source object id
  unsigned long timestamp;
};

struct PlayerPacket
{
  PacketHeader header;
  Vector3 position;
  Vector3 velocity;
  bool isRunning;
  bool isCrouching;
};

struct ItemPacket
{
  PacketHeader header;
  Vector3 position;
};

struct MonsterPacket
{
  PacketHeader header;
  Vector3 position;
};

struct WorldPacket
{

};


class Packet
{
public:
  Packet(void);
  virtual ~Packet(void);

  int getDataLength() { return MAX_PACKET_LENGTH; } //could make variable
  void* getDataPointer();
  
  char data[MAX_PACKET_LENGTH];

};
