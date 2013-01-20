#pragma once

#include "Packet.h"

class Packetable
{
public:
  Packetable(void);
  ~Packetable(void);

  virtual Packet extractPacket() = 0;
  virtual void integratePacket(Packet packet) = 0;
};

