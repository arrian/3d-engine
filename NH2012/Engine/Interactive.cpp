#include "Interactive.h"


Interactive::Interactive(InteractiveDesc desc)
  : Identifiable(this, desc.name, INTERACTIVE),
    Physical(),
    BasicComponent(),
    desc(desc)
{
}

//-------------------------------------------------------------------------------------
Interactive::~Interactive(void)
{
}

