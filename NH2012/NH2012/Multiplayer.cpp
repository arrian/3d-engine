#include "Multiplayer.h"

Multiplayer::Multiplayer(void)
{
  response = 0;

  std::string host = "127.0.0.1";
  unsigned short port = 30001;

  listener = boost::thread(boost::bind(&Multiplayer::listen, this, host, port));
}

Multiplayer::~Multiplayer(void)
{
}

/**
 * Listens to the server for world frame data
 */
void Multiplayer::listen(const std::string& host, unsigned short port)
{
  boost::asio::io_service io_service;
  Client client(io_service, host, port);

  //while(true)
  //{
  //  std::cout << "listening..." << std::endl;
  //  boost::this_thread::sleep(boost::posix_time::milliseconds(60));
  //}
}

/**
 * Gets the current world state and sends the current player state
 */
WorldData Multiplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  response++;
  WorldData testData;
  testData.timestamp = response;
  return testData;
}