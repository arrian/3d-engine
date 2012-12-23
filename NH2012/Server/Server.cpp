#include "Server.h"

#include "ScriptManager.h"

Server::Server(boost::asio::io_service& io_service, short port)
  : io_service(io_service),
    work(this->io_service),
    socket(io_service, udp::endpoint(udp::v4(), port)),
    world(),
    game(NULL),
    communication(NULL),
    inUpdate(false),
    inInput(false),
    turn(0),
    buffer(NULL)
{

  std::string resources;
  std::string plugins;

#ifdef _DEBUG
  resources = "resources_d.cfg";
  plugins = "plugins_d.cfg";
#else
  resources = "resources_d.cfg";
  plugins = "plugins_d.cfg";
#endif

  Ogre::Root* root = new Ogre::Root(plugins);

  Ogre::ConfigFile cf;
  cf.load(resources);
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while(seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
    }
  }

  //if(root->restoreConfig() || root->showConfigDialog())
  if(root->showConfigDialog())
  {
    root->initialise(false);
  }
  else throw NHException("failed to initialise");

  //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  //::Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(::Ogre::TFO_ANISOTROPIC);

  

  std::cout << "Server " << SERVER_VERSION << " using port " << socket.local_endpoint().port() << std::endl;
  world.setRoot(root);
  world.initialise("nh2012.ini");

  //start threads
  game = new boost::thread(&Server::update, this);
  communication = new boost::thread(&Server::serve, this);
}


Server::~Server(void)
{
  delete game;
  delete communication;
  delete buffer;
}

void Server::run()
{
  while(true)
  {
    std::string input;
    getline(std::cin, input);

    if(input == "exit") 
    {
      game->interrupt();
      communication->interrupt();
      return;
    }

    inInput = true;
    turn = 0;
    while(inUpdate == true && turn == 0) {}//wait for update to release lock
    
    //critical section
    try
    {
      world.getScriptManager()->execute(input);
    }
    catch(NHException e)
    {
      std::cout << e.what() << std::endl;
    }

    inInput = false;
  }
}

void Server::serve()
{
  try
  {
    io_service.run();
  }
  catch(boost::thread_interrupted const&)
  {

  }
}

void Server::update()//thread this method
{
  try
  {
    while(true)
    {
      inUpdate = true;
      turn = 1;
      while(inInput == true && turn == 1) {}//wait for input to release lock
      
      //critical section
      world.update(0.015);//make variable timestep

      //if no window then we need this to update ogre root
      //my_ogre.pRoot->_fireFrameStarted();
      //my_ogre.pSceneManager->_updateSceneGraph(NULL);
      //my_ogre.pRoot->_fireFrameEnded();

      inUpdate = false;

      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
  }
  catch(boost::thread_interrupted const&)
  {

  }
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
      boost::asio::buffer(data, MAX_PACKET_LENGTH), sender_endpoint,
      boost::bind(&Server::handleReceive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
  }
}

void Server::handleSend(const boost::system::error_code& error, size_t bytes_sent)
{
  socket.async_receive_from(
    boost::asio::buffer(data, MAX_PACKET_LENGTH), sender_endpoint,
    boost::bind(&Server::handleReceive, this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void Server::execute(std::string command)
{
  world.getScriptManager()->execute(command);
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