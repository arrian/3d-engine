#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "../Engine/World.h"

using boost::asio::ip::udp;

/**
 * Server implementation for world engine.
 **/
class Server
{
public:
  Server(unsigned short port);
  ~Server(void);

  void run();
private:
  void receive(const boost::system::error_code& error, std::size_t bytes);
  void send(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes);
  
  boost::asio::io_service io_service;
  boost::asio::ip::udp::socket socket;
  boost::asio::ip::udp::endpoint endpoint;//sender endpoint
  
  enum {max_length = 1024};
  char data[max_length];
  
  World world;
  unsigned short port;
};

/**
 * Entry point for the dedicated server.
 * Usage is 'server port'.
**/
int main(int argc, char* argv[])
{
  if(argc != 2) std::cerr << "Usage: server <port>" << std::endl;
  unsigned short port = boost::lexical_cast<unsigned short>(argv[1]);
  Server server(port);
  server.run();
}

