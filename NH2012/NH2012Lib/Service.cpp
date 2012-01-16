#include "Service.h"


Service::Service(boost::asio::io_service& io_service, unsigned short port) : 
  socket(io_service, udp::endpoint(udp::v4(), port))
{

}


Service::~Service(void)
{
}
