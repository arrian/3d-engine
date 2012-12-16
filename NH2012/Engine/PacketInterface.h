#pragma once

#include "Packet.h"

class PacketInterface
{
public:
  PacketInterface(void);
  ~PacketInterface(void);

  virtual Packet extractPacket() = 0;
  virtual void integratePacket(Packet packet) = 0;
};

