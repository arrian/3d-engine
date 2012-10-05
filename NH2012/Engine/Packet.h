#pragma once
class Packet
{
public:
  Packet(void);
  virtual ~Packet(void);

  unsigned long timestamp;
};

