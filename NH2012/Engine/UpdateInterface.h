#pragma once

#include "Packet.h"

/**
 * Interface for network game updates using packets.
 */
class UpdateInterface
{
public:
  UpdateInterface(void);
  virtual ~UpdateInterface(void);

  virtual void update(Packet packet) = 0;
};

