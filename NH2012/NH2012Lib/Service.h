#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;

class Service
{
public:
  Service(boost::asio::io_service& io_service, unsigned short port);
  Service(boost::asio::io_service& io_service, std::string host, unsigned short port);
  ~Service(void);

protected:
  virtual void startReceive() = 0;
  virtual void handleReceive(const boost::system::error_code& error, std::size_t bytes) = 0;

  virtual void handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes) = 0;

  udp::socket socket;
  udp::endpoint endpoint;
  boost::array<char, 128> buffer;
};

