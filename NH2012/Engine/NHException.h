#pragma once
#include <iostream>

class NHException : public std::exception
{
public:
  NHException(const char* message);
  ~NHException(void);
};



