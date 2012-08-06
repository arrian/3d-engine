#include "NHException.h"


//-------------------------------------------------------------------------------------
NHException::NHException(const char* message)
  : std::exception(message)
{
}


//-------------------------------------------------------------------------------------
NHException::~NHException(void)
{
}


