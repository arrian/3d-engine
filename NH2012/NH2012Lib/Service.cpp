#include "Service.h"

Service::Service(boost::asio::io_service& io_service, unsigned short port) : 
  socket(io_service, boost::asio::ip::udp::endpoint(udp::v4(), port))
{

}

Service::Service(boost::asio::io_service& io_service, std::string host, unsigned short port) : 
  endpoint(boost::asio::ip::address::from_string(host), port),
  socket(io_service, endpoint)
{

}


Service::~Service(void)
{
}
