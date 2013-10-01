#include "NHException.h"


//-------------------------------------------------------------------------------------
NHException::NHException(const char* message)
  : std::exception(message)
{
  std::cout << message << std::endl;
}


//-------------------------------------------------------------------------------------
NHException::~NHException(void)
{
}


