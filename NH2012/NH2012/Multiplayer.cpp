#include "Multiplayer.h"

Multiplayer::Multiplayer(void)
{
  response = 0;

  boost::asio::io_service io_service;
  listener = boost::thread(boost::bind(&Multiplayer::listen, this, &io_service, "127.0.0.1", "30001"));
}

Multiplayer::~Multiplayer(void)
{
}

/**
 * Listens to the server for world frame data
 */
void Multiplayer::listen(boost::asio::io_service& io_service, const std::string& host, const std::string& service)
{
  Receiver receiver(io_service);

  // Resolve the host name into an IP address.
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(host, service);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
    resolver.resolve(query);
  boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

  // Start an asynchronous connect operation.
  receiver.socket().async_connect(endpoint,
    boost::bind(&Multiplayer::handleConnect, this, receiver, 
    boost::asio::placeholders::error, ++endpoint_iterator));

  //while(true)
  //{
  //  std::cout << "listening..." << std::endl;
  //  boost::this_thread::sleep(boost::posix_time::milliseconds(60));
  //}
}

void Multiplayer::handleConnect(Receiver& receiver, const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
  if (!e)
  {
    // Successfully established connection. Start operation to read the list
    // of stocks. The connection::async_read() function will automatically
    // decode the data that is read from the underlying socket.
    receiver.async_read(frames,
        boost::bind(&Multiplayer::handleRead, this,
          boost::asio::placeholders::error));
  }
  else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
  {
    // Try the next endpoint.
    receiver.socket().close();
    boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
    receiver.socket().async_connect(endpoint,
        boost::bind(&Multiplayer::handleConnect, this, receiver,
          boost::asio::placeholders::error, ++endpoint_iterator));
  }
  else
  {
    std::cerr << e.message() << std::endl;
  }
}

void Multiplayer::handleRead(const boost::system::error_code& e)
{
  if (!e)
  {
    // Print out the data that was received.
    for (std::size_t i = 0; i < frames.size(); ++i)
    {
      std::cout << "Frame number " << i << "\n";
    }
  }
  else
  {
    // An error occurred.
    std::cerr << e.message() << std::endl;
  }

  // Since we are not starting a new operation the io_service will run out of
  // work to do and the client will exit.
}

/**
 * Gets the current world state
 */
WorldData Multiplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  response++;
  WorldData testData;
  testData.timestamp = response;
  return testData;
}