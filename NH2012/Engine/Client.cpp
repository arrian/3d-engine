#include "Client.h"

Client::Client(boost::asio::io_service& io_service, std::string host, unsigned short port)
{
  startReceive();
  io_service.run();
}

Client::~Client(void)
{
}

void Client::startReceive()
{
}

void Client::handleReceive(const boost::system::error_code& error, std::size_t bytes)
{
  if (!error || error == boost::asio::error::message_size)
  {
    boost::shared_ptr<std::string> message(
      new std::string("client"));

    startReceive();
  }
}

void Client::handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}
