#include "Multiplayer.h"


Multiplayer::Multiplayer(void)
{
  response = 0;

  //starting threaded server world frame request
  std::cout << "Beginning threaded server frame requester..." << std::endl;
  FrameRequester requester();
  boost::thread thrd(requester);
  thrd.join();
  std::cout << "Started server frame requester.";
}


Multiplayer::~Multiplayer(void)
{
}

//requests a frame from the server
void Multiplayer::generateFrame()
{
  std::cout << "debug: requesting frame" << std::endl;
  response++;
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