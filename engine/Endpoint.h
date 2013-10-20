
#pragma once

#include "Id.h"

class Player;

class Endpoint
{
public:
  bool authority;
  Id<Player> playerId;

  Endpoint(bool authority = false, Id<Player> playerId = Id<Player>())
    : authority(authority),
      playerId(playerId)
  {
  }

};


