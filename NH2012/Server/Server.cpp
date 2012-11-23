#include "Server.h"


Server::Server(boost::asio::io_service& io_service, short port)
  : io_service(io_service),
    socket(io_service, udp::endpoint(udp::v4(), port))
{
  std::cout << "Server " << SERVER_VERSION << " using port " << socket.local_endpoint().port() << std::endl;
  std::cin.get();
}


Server::~Server(void)
{
}

void Server::run()
{
  io_service.run();
}

void Server::handleReceive(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error && bytes_recvd > 0)
  {
    socket.async_send_to(
      boost::asio::buffer(data, bytes_recvd), sender_endpoint,
      boost::bind(&Server::handleSend, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    socket.async_receive_from(
      boost::asio::buffer(data, max_length), sender_endpoint,
      boost::bind(&Server::handleReceive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  }
}

void Server::handleSend(const boost::system::error_code& error, size_t bytes_sent)
{
  socket.async_receive_from(
    boost::asio::buffer(data, max_length), sender_endpoint,
    boost::bind(&Server::handleReceive, this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

int main(int argc, char* argv[])
{
  boost::asio::io_service io_service;
  Server* server;

  if(argc > 1) server = new Server(io_service, atoi(argv[1]));//.setPort(atoi(argv[1]));
  else server = new Server(io_service);
  
  server->run();

  if(server) delete server;

  return 0;
}