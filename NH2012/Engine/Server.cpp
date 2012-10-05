#include "Server.h"

#include <OgreFrameListener.h>

Server::Server(unsigned short port)
  : world(),//initialise server world
    io_service(),
    port(port),
    socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))    
{
}

Server::~Server(void)
{
}

void Server::run()
{
  std::cout << "Starting server on port " << port << std::endl;
  socket.async_receive_from(boost::asio::buffer(data,max_length), endpoint, boost::bind(&Server::receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  io_service.run();
  
  //need to iterate the world here/ on a thread
  /*
  Ogre::FrameEvent evt;
  evt.timeSinceLastEvent = 1.0;
  evt.timeSinceLastFrame = 1.0;
  while(true) world.frameRenderingQueued(evt);
  */
}

void Server::receive(const boost::system::error_code& error, std::size_t bytes)
{
  if (!error || error == boost::asio::error::message_size)
  {

    boost::shared_ptr<std::string> message(new std::string("server"));
  }
}

void Server::send(boost::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes)
{

}

