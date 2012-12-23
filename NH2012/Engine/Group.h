

#pragma once

enum Group
{
  NONE = 0,
  PLAYER_SELF = (1<<0),
  PLAYER_OTHER = (1<<1),
  MONSTER = (1<<2),
  ITEM = (1<<3),
  INTERACTIVE = (1<<4),
  ARCHITECTURE = (1<<5),

  EXCLUDE_SELF = PLAYER_OTHER | MONSTER | ITEM | INTERACTIVE | ARCHITECTURE
};

