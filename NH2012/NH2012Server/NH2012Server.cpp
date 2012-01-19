// NH2012Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <Server.h>
#include <boost/exception/all.hpp>

int main(int argc, char* argv[])
{
  boost::asio::io_service io_service;
  Server server(io_service, 30001);
	return 0;
}

