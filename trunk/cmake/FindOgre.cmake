# - Test for Ogre
# Once loaded this will define
#   Ogre_FOUND        - system has Ogre
#   Ogre_INCLUDE_DIR  - include directory for Ogre
#   Ogre_LIB_RELEASE  - release libraries directory
#   Ogre_LIB_DEBUG    - debug libraries directory
#   Ogre_SAMPLES_DIR  - debug libraries directory
#   Ogre_BIN_DIR      - ogre binary directory 

set(Ogre_FOUND "NO")
set(OGRE_HOME "$ENV{OGRE_HOME}")

if(WIN32)
    if(MSVC80)
        find_path(Ogre_INCLUDE_DIR "Ogre.h"
            "${OGRE_HOME}/include")

        find_path(Ogre_SAMPLES_DIR "include/Bezier.h"
            "${OGRE_HOME}/samples")

        find_path(Ogre_BIN_DIR "release/ogremain.dll"
            "${OGRE_HOME}/bin")

        find_path(Ogre_LIB_RELEASE "OgreMain.lib" 
	        "${OGRE_HOME}/lib")

        find_path(Ogre_LIB_DEBUG "OgreMain_d.lib" 
            "${OGRE_HOME}/lib")
    else(MSVC80)
        find_path(Ogre_INCLUDE_DIR "Ogre.h"
            "${OGRE_HOME}/include")

        find_path(Ogre_SAMPLES_DIR "include/Bezier.h"
            "${OGRE_HOME}/samples")

        find_path(Ogre_BIN_DIR "release/ogremain.dll"
            "${OGRE_HOME}/bin")

        find_path(Ogre_LIB_RELEASE "OgreMain.lib" 
	        "${OGRE_HOME}/lib")

        find_path(Ogre_LIB_DEBUG "OgreMain_d.lib" 
            "${OGRE_HOME}/lib")
    endif(MSVC80)
else(WIN32)
    find_path(Ogre_INCLUDE_DIR "Ogre.h"
        "/usr/include/OGRE")

    find_path(Ogre_LIB_RELEASE "libOgreMain.so"
	    "/usr/lib")

    find_path(Ogre_LIB_DEBUG "libOgreMain.so" 
        "/usr/lib")
endif(WIN32)

if(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)
	set(Ogre_FOUND "YES")
endif(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)

if (Ogre_FOUND)
    if (NOT Ogre_FIND_QUIETLY)
        message (STATUS "Found Ogre: ${Ogre_INCLUDE_DIR}, ${Ogre_LIB_RELEASE}, ${Ogre_LIB_DEBUG}")
    endif (NOT Ogre_FIND_QUIETLY)
else (Ogre_FOUND)
    if (Ogre_FIND_REQUIRED)
        message (FATAL_ERROR "Could not find Ogre")
    else (Ogre_FIND_REQUIRED)
        message (STATUS "Could not find Ogre")
    endif (Ogre_FIND_REQUIRED)
endif (Ogre_FOUND)
