#include "Multiplayer.h"

Multiplayer::Multiplayer(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow) 
  : Game(renderWindow)
{
  std::string host = "127.0.0.1";
  unsigned short port = 30001;

  listener = boost::thread(boost::bind(&Multiplayer::listen, this, host, port));
}

Multiplayer::~Multiplayer(void)
{
}

/**
 * Listens to the server for world frame data
 */
void Multiplayer::listen(const std::string& host, unsigned short port)
{
  boost::asio::io_service io_service;
  //client = new Client(io_service, host, port);

  //while(true)
  //{
  //  std::cout << "listening..." << std::endl;
  //  boost::this_thread::sleep(boost::posix_time::milliseconds(60));
  //}
}

/**
 * Gets the current world state and sends the current player state
 */
void Multiplayer::update(double elapsedSeconds)
{
}

void Multiplayer::injectKeyDown(const OIS::KeyEvent &arg)
{

}

void Multiplayer::injectKeyUp(const OIS::KeyEvent &arg)
{

}

void Multiplayer::injectMouseMove(const OIS::MouseEvent &arg)
{

}

void Multiplayer::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void Multiplayer::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

}

void Multiplayer::notify(Ogre::String comment)
{
  std::cout << comment << std::endl;
}

void Multiplayer::sceneChanged()
{

}
