#include "Server.h"

Server::Server(boost::asio::io_service& io_service, unsigned short port) : 
  Service(io_service, port)
{
}

Server::~Server(void)
{
}

void Server::startReceive()
{

}

void Server::handleReceive(const boost::system::error_code& error, std::size_t bytes)
{

}

void Server::handleSend(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}

