
set(EXECUTABLE_NAME_ENGINE "Engine" CACHE STRING "Test" FORCE)

project (${EXECUTABLE_NAME_ENGINE})

file(GLOB SOURCE_GLOB
  "./*.h"
  "./*.cpp"
)

add_library(${EXECUTABLE_NAME_ENGINE} ${SOURCE_GLOB})


#Release Include - C:\Program Files %28x86%29\FMOD SoundSystem\FMOD Programmers API Windows\api\inc;C:\Dev\Nethack2012\NH2012\NH2012Lib;include;$(OGRE_HOME)\include;$(OGRE_SDK)\include;$(OGRE_DEPS)\include\OIS;$(OGRE_SDK)\include\OGRE;$(OGRE_HOME)\Samples\Common\include;$(Boost_ROOT);C:\Dev\Ogre3D\Samples\Common\include;C:\Dev\PhysX\Include;C:\Dev\PhysX\Samples\PxToolkit\include;C:\Dev\Ogre3D\Components\Terrain\include;C:\Dev\OgreHydrax\libhydrax-0.5.4\include;%(AdditionalIncludeDirectories)

#Debug Include - C:\Program Files %28x86%29\FMOD SoundSystem\FMOD Programmers API Windows\api\inc;C:\Dev\Nethack2012\NH2012\Engine;C:\Dev\Nethack2012\NH2012\NH2012Lib;include;$(OGRE_HOME)\include;$(OGRE_SDK)\include;$(OGRE_DEPS)\include\OIS;$(OGRE_SDK)\include\OGRE;$(OGRE_HOME)\Samples\Common\include;$(Boost_ROOT);C:\Dev\Ogre3D\Samples\Common\include;C:\Dev\PhysX\Include;C:\Dev\PhysX\Samples\PxToolkit\include;C:\Dev\Ogre3D\Components\Terrain\include;C:\Dev\OgreHydrax\libhydrax-0.5.4\include;%(AdditionalIncludeDirectories)