#pragma once

/**
 * Defines an object that can be interacted with.
 */
class InteractiveInterface
{
public:
  virtual ~InteractiveInterface(void) {};

  virtual void interact() = 0;
};

