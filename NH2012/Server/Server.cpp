#include "Server.h"

Server::Server(boost::asio::io_service& io_service, unsigned short port)
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

  std::cout << "Done." << std::endl;
  std::cout << "Listening..." << std::endl;
}

void Server::handleReceive(const boost::system::error_code& error, std::size_t bytes)
{
  if (!error || error == boost::asio::error::message_size)
  {

    boost::shared_ptr<std::string> message(
      new std::string("server"));


    startReceive();
  }
}

void Server::handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}

