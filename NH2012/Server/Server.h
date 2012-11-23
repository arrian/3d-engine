#pragma once

#include <iostream>
#include <assert.h>
#include <cstdlib>

#include "boost/bind.hpp"
#include "boost/asio.hpp"

using boost::asio::ip::udp;

#define MAX_MESSAGE_LENGTH 1024
#define DEFAULT_PORT 22225
#define SERVER_VERSION "v0.1"

class Server
{
public:
  Server(boost::asio::io_service& io_service, short port = DEFAULT_PORT);
  virtual ~Server(void);
  void run();

  void handleReceive(const boost::system::error_code& error, size_t bytes_recvd);
  void handleSend(const boost::system::error_code& error, size_t bytes_sent);
private:
  boost::asio::io_service& io_service;
  udp::socket socket;
  udp::endpoint sender_endpoint;
  enum { max_length = MAX_MESSAGE_LENGTH };
  char data[max_length];
};

