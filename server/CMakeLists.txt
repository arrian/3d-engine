
set(EXECUTABLE_NAME_SERVER "Server" CACHE STRING "Test" FORCE)

project (${EXECUTABLE_NAME_SERVER})

file(GLOB SOURCE_GLOB
  "./*.h"
  "./*.cpp"
)


add_ogre_executable(${EXECUTABLE_NAME_SERVER} ${SOURCE_GLOB})


# Debug Include - C:\Dev\Nethack2012\NH2012\Engine;C:\Program Files %28x86%29\FMOD SoundSystem\FMOD Programmers API Windows\api\inc;C:\Dev\Nethack2012\NH2012\NH2012Lib;include;$(OGRE_HOME)\include;$(OGRE_SDK)\include;$(OGRE_DEPS)\include\OIS;$(OGRE_SDK)\include\OGRE;$(OGRE_HOME)\Samples\Common\include;$(Boost_ROOT);C:\Dev\Ogre3D\Samples\Common\include;C:\Dev\PhysX\Include;C:\Dev\OgreCrowd\include;C:\Dev\Ogre3D\Components\Terrain\include;C:\Dev\OgreHydrax\libhydrax-0.5.4\include;C:\Dev\PhysX\Samples\PxToolkit\include;%(AdditionalIncludeDirectories)

# Release Include - $(Boost_ROOT)

# Debug Libraries - kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies);OgreMain_d.lib;OgreTerrain_d.lib;OIS_d.lib;Engine.lib;PhysX3CHECKED_x86.lib;PhysX3CommonCHECKED_x86.lib;PhysX3CookingCHECKED_x86.lib;PhysX3CharacterKinematicCHECKED_x86.lib;PhysX3ExtensionsCHECKED.lib;PhysXVisualDebuggerSDKCHECKED.lib;PxToolkitCHECKED.lib;fmodex_vc.lib;Hydrax.lib;%(AdditionalDependencies)

# Release Libraries - kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)