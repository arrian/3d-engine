#pragma once

#include "Packet.h"

/**
 * Defines an object that can be converted to a network packet.
 * The template argument defines the target packet struct.
 */
template<typename PacketStruct>
class Packetable
{
public:
  Packetable(void) {}
  virtual ~Packetable(void) {}

  virtual PacketStruct extractPacket() = 0;
  virtual void integratePacket(PacketStruct packet) = 0;
};

