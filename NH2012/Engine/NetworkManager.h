#pragma once

#include <deque>

#include "Id.h"
#include "Packet.h"
#include "Container.h"

class World;
class Player;

class NetworkManager
{
public:
  NetworkManager();
  virtual ~NetworkManager(void);

  void update(double elapsedSeconds);

  void connect(std::string address);//connects to a server making this terminal a client (ie. not an authority)
  void disconnect(std::string address);
  void disconnect(Id<Endpoint> endpoint);

  void setBroadcast(bool broadcast);//broadcast availability
  void setIsAuthority(bool authority);
  void setMaxConnections(int maxConnections);//max number of client connections

  void setWorld(World* world) {this->world = world;}
  
protected:
  World* world;
  Container<Endpoint> endpoints;//the remote players

  void send();
  void receive();

private:
  bool hasConnections;
  bool authority;
  bool broadcast;
  int maxConnections;
  std::deque<Packet> received;

  Packet getNextPacket();

  void sendNextPackets();//client packet... sending essential player updates
  void sendNextAuthorityPackets();//server packet... broadcast updates

  void sendAcknowledge(Packet packet);
};

