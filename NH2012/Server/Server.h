#pragma once

#include <iostream>
#include <assert.h>
#include <cstdlib>

#include "boost/bind.hpp"
#include "boost/asio.hpp"
#include "boost/thread/thread.hpp"

#include <OgreDefaultHardwareBufferManager.h>

#include "../Engine/Console.h"
#include "../Engine/Packet.h"
#include "../Engine/SceneChangeListener.h"
#include "../Engine/World.h"

using boost::asio::ip::udp;

#define DEFAULT_PORT 22225
#define SERVER_VERSION "v0.1"

class Server
{
public:
  Server(boost::asio::io_service& io_service, short port);
  virtual ~Server(void);
  

  void handleReceive(const boost::system::error_code& error, size_t bytes_recvd);
  void handleSend(const boost::system::error_code& error, size_t bytes_sent);

  void run();

private:
  
  World* world;

  Ogre::DefaultHardwareBufferManager* buffer;

  boost::asio::io_service& io_service;
  boost::asio::io_service::work work;//stop the io_service from running out of work
  udp::socket socket;
  udp::endpoint sender_endpoint;

  char data[MAX_PACKET_LENGTH];

  boost::thread* game;//game update thread
  boost::thread* communication;//server communication thread

  void execute(std::string command);

  void update();
  void serve();

  //Peterson's Algorithm for mutual exclusion
  volatile bool inUpdate;
  volatile bool inInput;
  volatile int turn;//update or input turn

};

