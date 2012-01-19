#include "Client.h"

Client::Client(boost::asio::io_service& io_service, std::string host, unsigned short port) : 
  Service(io_service, port)
{
  startReceive();
  io_service.run();
}

Client::~Client(void)
{
}

void Client::startReceive()
{
  socket.async_receive_from(
    boost::asio::buffer(buffer), endpoint,
    boost::bind(&Client::handleReceive, this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void Client::handleReceive(const boost::system::error_code& error, std::size_t bytes)
{
  if (!error || error == boost::asio::error::message_size)
  {
    std::cout.write(buffer.data(), 128);

    boost::shared_ptr<std::string> message(
      new std::string("client"));

    socket.async_send_to(boost::asio::buffer(*message), endpoint,
      boost::bind(&Client::handleSend, this, message,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));

    startReceive();
  }
}

void Client::handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}
