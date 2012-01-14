#include "Multiplayer.h"


Multiplayer::Multiplayer(void)
{
  response = 0;

  //starting threaded server world frame request
  std::cout << "Beginning threaded server frame requester..." << std::endl;
  packetSocket = boost::thread(PacketSocket(1));
  packetSocket.join();
  std::cout << "Started server frame requester.";
}

Multiplayer::~Multiplayer(void)
{
}

WorldData Multiplayer::convertResponse()
{
  WorldData testData;
  testData.timestamp = response;
  return testData;
}

WorldData Multiplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  return convertResponse();
}