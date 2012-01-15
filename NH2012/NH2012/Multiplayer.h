#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <OgreFrameListener.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "Receiver.h"
#include "Client.h"

#include <World.h>
 
class Multiplayer :
  public Client
{
public:
  Multiplayer(void);
  ~Multiplayer(void);

  WorldData frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
  int response;
  void handleConnect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
  void handleRead(const boost::system::error_code& e);

  boost::thread listener;
  void listen(boost::asio::io_service& io_service, const std::string& host, const std::string& service);

  /// The data received from the server.
  std::vector<WorldData> frames;
};

