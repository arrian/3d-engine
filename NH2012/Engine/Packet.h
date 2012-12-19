
#define MAX_PACKET_LENGTH 1024

class Packet
{
public:
  Packet(void);
  virtual ~Packet(void);

  char data[MAX_PACKET_LENGTH];
};
