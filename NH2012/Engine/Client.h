#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;

class Client
{
public:
  Client(boost::asio::io_service& io_service, std::string host, unsigned short port);
  ~Client(void);

private:
  void startReceive();
  void handleReceive(const boost::system::error_code& error, std::size_t bytes);

  void handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes);
};

