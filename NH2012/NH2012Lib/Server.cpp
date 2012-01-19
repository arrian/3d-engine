#include "Server.h"

Server::Server(boost::asio::io_service& io_service, unsigned short port) : 
  Service(io_service, port)
{
  std::cout << "Starting server...";
  startReceive();
  io_service.run();
}

Server::~Server(void)
{
}

void Server::startReceive()
{
  socket.async_receive_from(
  boost::asio::buffer(buffer), endpoint,
  boost::bind(&Server::handleReceive, this,
  boost::asio::placeholders::error,
  boost::asio::placeholders::bytes_transferred));

  std::cout << "Done." << std::endl;
  std::cout << "Listening..." << std::endl;
}

void Server::handleReceive(const boost::system::error_code& error, std::size_t bytes)
{
  if (!error || error == boost::asio::error::message_size)
  {
    std::cout.write(buffer.data(), 128);

    boost::shared_ptr<std::string> message(
      new std::string("server"));

    socket.async_send_to(boost::asio::buffer(*message), endpoint,
      boost::bind(&Server::handleSend, this, message,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));

    startReceive();
  }
}

void Server::handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}

