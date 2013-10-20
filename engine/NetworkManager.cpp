#include "NetworkManager.h"

#include "Player.h"
#include "World.h"
#include "Endpoint.h"
#include "Scene.h"


NetworkManager::NetworkManager()
  : hasConnections(false),
    authority(false),
    broadcast(false),
    maxConnections(16),
    endpoints()
{

}

NetworkManager::~NetworkManager(void)
{
}

void NetworkManager::connect(std::string address)
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
  int initialReceived = received.size();

  while(initialReceived > 0)
  {
    Packet packet = getNextPacket();
    //TODO: receive packet from remote endpoints

    PacketHeader* header = packet.getHeader();
    boost::shared_ptr<Endpoint> endpoint = endpoints.get(header->sourceId);

    if(header->type == PLAYER_UPDATE)
    {
      PlayerPacket* playerPacket = packet.getDataPointer<PlayerPacket>();
      boost::shared_ptr<Scene> scene = getWorld()->getScene(playerPacket->sceneId);

      if(!endpoint)
      {
        endpoints.insert(header->sourceId, boost::shared_ptr<Endpoint>(new Endpoint(false, playerPacket->playerId)));
        endpoint = endpoints.get(header->sourceId);
        if(!endpoint) throw NHException("Endpoint could not be created");
      }

      if(scene)
      {
        boost::shared_ptr<Player> player = scene->get<Player>(playerPacket->playerId);
        if(player)
        {
          player->integratePacket(*playerPacket);
        }
        else
        {
          scene->addPlayer(boost::shared_ptr<Player>(new Player(PlayerDesc(),getWorld())), playerPacket->position, playerPacket->lookAt, playerPacket->playerId);
          player = scene->get<Player>(endpoint->playerId);
          if(!player) throw NHException("Player could not be created");
        }
      }
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
  getWorld()->getPlayer()->extractPacket();//TODO: send this packet
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


