#pragma once

#include "Packet.h"

class PacketInterface
{
public:
  PacketInterface(void);
  ~PacketInterface(void);

  Packet extractPacket() = 0;
  void integratePacket(Packet packet) = 0;
};

