
set(EXECUTABLE_NAME_EDITOR "Editor" CACHE STRING "Test" FORCE)

project (${EXECUTABLE_NAME_EDITOR})

file(GLOB SOURCE_GLOB
  "./*.h"
  "./*.cpp"
)

add_ogre_executable(${EXECUTABLE_NAME_EDITOR} ${SOURCE_GLOB})

#Debug Include - .\GeneratedFiles;$(QTDIR)\include;.\GeneratedFiles\$(ConfigurationName);$(QTDIR)\include\QtCore;$(QTDIR)\include\QtGui;.\;$(QTDIR)\include\QtOpenGL;C:\Program Files %28x86%29\FMOD SoundSystem\FMOD Programmers API Windows\api\inc;C:\Dev\Nethack2012\NH2012\NH2012Lib;include;$(OGRE_HOME)\include;$(OGRE_SDK)\include;$(OGRE_DEPS)\include\OIS;$(OGRE_SDK)\include\OGRE;$(OGRE_HOME)\Samples\Common\include;$(Boost_ROOT);C:\Dev\Ogre3D\Samples\Common\include;C:\Dev\PhysX\Include;C:\Dev\PhysX\Samples\PxToolkit\include;C:\Dev\Ogre3D\Components\Terrain\include;%(AdditionalIncludeDirectories)

#Release Include - .\GeneratedFiles;$(QTDIR)\include;.\GeneratedFiles\$(ConfigurationName);$(QTDIR)\include\QtCore;$(QTDIR)\include\QtGui;.\;$(QTDIR)\include\QtOpenGL;%(AdditionalIncludeDirectories)

#Debug Libraries - qtmaind.lib;QtCored4.lib;QtGuid4.lib;QtOpenGLd4.lib;opengl32.lib;glu32.lib;%(AdditionalDependencies);OgreMain_d.lib;OgreTerrain_d.lib;OIS_d.lib;Engine.lib;PhysX3CHECKED_x86.lib;PhysX3CommonCHECKED_x86.lib;PhysX3CookingCHECKED_x86.lib;PhysX3CharacterKinematicCHECKED_x86.lib;PhysX3ExtensionsCHECKED.lib;PhysXVisualDebuggerSDKCHECKED.lib;PhysXProfileSDKCHECKED.lib;PxToolkitCHECKED.lib;fmodex_vc.lib

#Release Libraries - qtmain.lib;QtCore4.lib;QtGui4.lib;QtOpenGL4.lib;opengl32.lib;glu32.lib;OgreMain.lib;OgreTerrain.lib;OIS.lib;Engine.lib;PhysX3_x86.lib;PhysX3Common_x86.lib;PhysX3Cooking_x86.lib;PhysX3CharacterKinematic_x86.lib;PhysX3Extensions.lib;PhysXVisualDebuggerSDK.lib;PhysXProfileSDK.lib;PxToolkit.lib;fmodex_vc.lib;Hydrax.lib;%(AdditionalDependencies)