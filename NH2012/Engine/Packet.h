
#define MAX_PACKET_LENGTH 1024
#define HASH_LENGTH 10

enum PacketType
{
  ACKNOWLEDGE,//acknowledge an important notification
  UPDATE,//general purpose unimportant packet
  NOTIFY,//requires acknowledge packet
  REQUEST,//requires an update reply
  KEEP_ALIVE//sends if nothing else being sent to maintain connection
};

class Packet
{
public:
  Packet(void);
  virtual ~Packet(void);

  int getDataLength() { return MAX_PACKET_LENGTH; } //could make variable
  char* getDataPointer();

  char hash[HASH_LENGTH];//packet distinct hash for acknowledgments
  PacketType type;
  
  char data[MAX_PACKET_LENGTH];

};
