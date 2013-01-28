#include "NetworkManager.h"

#include "Player.h"
#include "World.h"


NetworkManager::NetworkManager()
  : hasConnections(false),
    authority(false),
    broadcast(false),
    maxConnections(8)
{

}

NetworkManager::~NetworkManager(void)
{
}

void NetworkManager::setIsAuthority(bool authority)
{
  this->authority = authority;
}

void NetworkManager::update(double elapsedSeconds)
{
  if(hasConnections)
  {
    receive();
    send();
  } 
}

void NetworkManager::send()
{
  if(authority) sendNextAuthorityPackets();
  else sendNextPackets();
}

void NetworkManager::receive()
{
  int initialReceived = received.size();//TODO: check bounds

  while(initialReceived > 0)
  {
    Packet packet = getNextPacket();
    //TODO: receive packet from remote endpoints

    PacketHeader* header = reinterpret_cast<PacketHeader*>(packet.getDataPointer());

    if(header->type == PLAYER_UPDATE)
    {
      PlayerPacket* playerPacket = reinterpret_cast<PlayerPacket*>(packet.getDataPointer());
      //TODO: get player id from endpoint not from the packet
      if(remotePlayers.count(playerPacket->header.id) == 0) throw NHException("the specified network player found in packet does not exist");
      remotePlayers.find(playerPacket->header.id)->second->integratePacket(*playerPacket);
    }
    else if(header->type == PLAYER_JOIN)
    {
      //create player in world
      //add player to map of remotePlayers
      //TODO: implement
    }
    else if(header->type == PLAYER_LEFT)
    {
      //remove player from remotePlayers
      //TODO: implement
    }
    else if(header->type == ACKNOWLEDGE)
    {
      //TODO: implement
      //remove acknowledge request from ...
    }

    initialReceived--;
  }
}

Packet NetworkManager::getNextPacket()
{
  Packet packet = received.front();
  received.pop_front();
  return packet;
}

void NetworkManager::sendNextPackets()//client packet... sending essential player updates
{
  world->getPlayer()->extractPacket();//TODO: send this packet
  //TODO: implement
}

void NetworkManager::sendNextAuthorityPackets()//server packet... broadcast updates
{
  static int NUMBER_OF_PACKETS = 5;//max packets to send per update
  for(int i = 0; i < NUMBER_OF_PACKETS; i++)
  {
    //TODO: implement  
  }
}

void NetworkManager::sendAcknowledge(Packet packet)
{
  //TODO: implement
}


