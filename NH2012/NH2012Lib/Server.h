#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "Service.h"

using boost::asio::ip::udp;

class Server : 
  public Service
{
public:
  Server(boost::asio::io_service& io_service, unsigned short port);
  ~Server(void);

private:
  void startReceive();
  void handleReceive(const boost::system::error_code& error, std::size_t bytes);

  void handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes);
};

