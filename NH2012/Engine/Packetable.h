#pragma once

#include "Packet.h"

template<typename PacketStruct>
class Packetable
{
public:
  Packetable(void) {}
  virtual ~Packetable(void) {}

  virtual PacketStruct extractPacket() = 0;
  virtual void integratePacket(PacketStruct packet) = 0;
};

