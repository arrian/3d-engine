#pragma once

#include <deque>

#include "Packet.h"

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
  void disconnect(int playerID);

  void setBroadcast(bool broadcast);//broadcast availability
  void setIsAuthority(bool authority);
  void setMaxConnections(int maxConnections);//max number of client connections
  
protected:
  World* world;
  std::map<int, Player*> remotePlayers;//change int id to connection endpoint for the server

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

