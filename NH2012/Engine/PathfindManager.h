#pragma once

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef WIN32
#	include <io.h>
#else
#	include <dirent.h>
#endif

#include "Recast.h"

class PathfindManager
{
public:
  PathfindManager(void);
  virtual ~PathfindManager(void);
};

