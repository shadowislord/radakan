# - Test for Ogre
# Once loaded this will define
#   Ogre_FOUND        - system has Ogre
#   Ogre_INCLUDE_DIR  - include directory for Ogre
#   Ogre_LIB_RELEASE  - release libraries directory
#   Ogre_LIB_DEBUG    - debug libraries directory

set(Ogre_FOUND "NO")

if(WIN32)
    find_path(Ogre_INCLUDE_DIR "Ogre.h"
        "D:/ScatteredLands/OgreSDK_1_2_5/include"
        "D:/ScatteredLands/OgreSDK/include" 
        "E:/Projects/ScatteredLands/OgreSDK/include" 
    	"C:/OgreSDK/include")

    find_path(Ogre_LIB_RELEASE "OgreMain.lib" 
	    "D:/Projects/ScatteredLands/OgreSDK_1_2_5/lib"
        "D:/ScatteredLands/OgreSDK/lib" 
        "E:/Projects/ScatteredLands/OgreSDK/lib" 
        "C:/OgreSDK/lib")

    find_path(Ogre_LIB_DEBUG "OgreMain_d.lib" 
        "D:/ScatteredLands/OgreSDK/lib" 
        "D:/ScatteredLands/OgreSDK_1_2_5/lib"
        "E:/Projects/ScatteredLands/OgreSDK/lib" 
        "C:/OgreSDK/lib")

    if(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)
        set(Ogre_FOUND "YES")
    endif(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)
else(WIN32)
    find_path(Ogre_INCLUDE_DIR "Ogre.h"
        "/usr/include/OGRE")

    find_path(Ogre_LIB_RELEASE "libOgreMain.so"
	    "/usr/lib")

    find_path(Ogre_LIB_DEBUG "libOgreMain.so" 
        "/usr/lib")

    if(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)
        set(Ogre_FOUND "YES")
    endif(Ogre_INCLUDE_DIR AND Ogre_LIB_RELEASE AND Ogre_LIB_DEBUG)
endif(WIN32)

if (Ogre_FOUND)
   if (NOT Ogre_FIND_QUIETLY)
      message(STATUS "Found Ogre: ${Ogre_LIBRARIES}")
   endif (NOT Ogre_FIND_QUIETLY)
else (Ogre_FOUND)
   if (Ogre_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Ogre")
   endif (Ogre_FIND_REQUIRED)
endif (Ogre_FOUND)
