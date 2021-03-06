

#pragma once

enum Group
{
  UNDEFINED = 0,
  NONE = (1<<0),
  PLAYER_SELF = (1<<1),
  PLAYER_OTHER = (1<<2),
  CREATURE = (1<<3),
  ITEM = (1<<4),
  INTERACTIVE = (1<<5),
  ARCHITECTURE = (1<<6),

  PLAYERS = PLAYER_OTHER | PLAYER_SELF,
  EXCLUDE_ARCHITECTURE = PLAYERS | CREATURE | ITEM | INTERACTIVE,
  EXCLUDE_SELF = PLAYER_OTHER | CREATURE | ITEM | INTERACTIVE | ARCHITECTURE,

  ALL = PLAYERS | CREATURE | ITEM | INTERACTIVE | ARCHITECTURE | NONE
};

