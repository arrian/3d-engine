#include "Bar.h"

Bar::Bar(void)
{
}

Bar::Bar(int max)
{
  this->max = max;
  current = max;
}

Bar::~Bar(void)
{
}

int Bar::getCurrent()
{
  return current;
}

int Bar::getMax()
{
  return max;
}

void Bar::increaseCurrent(int amount)
{
  current += amount;
}

void Bar::increaseMax(int amount)
{
  max += amount;
}