cmake_minimum_required (VERSION 2.8)



#Makes the distribution directories 
function(prepare_dir)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
endfunction()

#Sets CMake requirements.
function(prepare_cmake)
  if (CMAKE_BUILD_TYPE STREQUAL "")
    # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
    # differentiation between debug and release builds.
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
  endif ()
   
  set(CMAKE_DEBUG_POSTFIX "_d")
   
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

endfunction()

#Finds Ogre
function(prepare_ogre)

	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/include
	)

  find_package(OGRE REQUIRED)
  
  include_directories(${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
)

endfunction()

#Finds OIS.
function(prepare_ois)
  find_package(OIS REQUIRED)
   
  if(NOT OIS_FOUND)
    message(SEND_ERROR "Failed to find OIS.")
  endif()
  
  include_directories(${OIS_INCLUDE_DIRS})
   
endfunction()

#Prepares Boost.
function(prepare_boost)
  set(Boost_USE_STATIC_LIBS TRUE)

  set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
  # Components that need linking (NB does not include header-only components like bind)
  set(OGRE_BOOST_COMPONENTS thread date_time)
  find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
  if (NOT Boost_FOUND)
    # Try again with the other type of libs
    set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
  endif()
  find_package(Boost QUIET)

  # Set up referencing of Boost
  include_directories(${Boost_INCLUDE_DIR})
  add_definitions(-DBOOST_ALL_NO_LIB)

endfunction()


function(add_ogre_executable executable_name source_glob)

  add_executable(${executable_name} WIN32 ${source_glob})

  set_target_properties(${executable_name} PROPERTIES DEBUG_POSTFIX _d)

  target_link_libraries(${executable_name} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES})

  # post-build copy for win32
  add_custom_command( TARGET ${executable_name} PRE_BUILD
    COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
  add_custom_command( TARGET ${executable_name} POST_BUILD
    COMMAND copy \"$(TargetPath)\" .\\dist\\bin )  

  install(TARGETS ${executable_name}
  RUNTIME DESTINATION bin
  CONFIGURATIONS All)

endfunction()


function(prepare_binaries)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
		DESTINATION bin
		CONFIGURATIONS Debug
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
    ${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D11.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/OIS.dll
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
    ${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D11_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
		${OGRE_PLUGIN_DIR_DBG}/OIS_d.dll
		DESTINATION bin
		CONFIGURATIONS Debug
	)
 
   install(FILES ${Boost_DATE_TIME_LIBRARY_RELEASE}
      ${Boost_THREAD_LIBRARY_RELEASE}
      DESTINATION bin
      CONFIGURATIONS Release RelWithDebInfo
   )
 
   install(FILES ${Boost_DATE_TIME_LIBRARY_DEBUG}
      ${Boost_THREAD_LIBRARY_DEBUG}
      DESTINATION bin
      CONFIGURATIONS Debug
   )

endfunction()



 











project(Main)

add_subdirectory(NH2012)#change name to Game
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Server)

prepare_dir()
prepare_cmake()
prepare_ogre()
prepare_ois()
prepare_boost()
prepare_binaries()


