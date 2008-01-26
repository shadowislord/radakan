# - Test for Ode
# Once loaded this will define
#   Ode_FOUND        - system has Ode
#   Ode_INCLUDE_DIR  - include directory for Ode
#   Ode_LIB_RELEASE  - release library directory   
#   Ode_LIB_DEBUG    - release library directory   

find_package(Ogre)

if(WIN32)
    find_path(Ode_INCLUDE_DIR "ode/Ode.h"
        ${Ogre_INCLUDE_DIR}
        "D:/Projects/Radakan/ode_0_8/include"        
        "E:/Projects/ScatteredLands/ode-0.8/include"        
	    "C:/ode/include")

    find_path(Ode_LIB_RELEASE "ode.lib" 
        ${Ogre_LIB_RELEASE}
        "D:/Projects/Radakan/ode_0_8/lib/releaselib" 
        "E:/Projects/ScatteredLands/ode-0.8/lib/releaselib"        
        "C:/ode/lib/releaselib")

    find_path(Ode_LIB_DEBUG "ode.lib" 
        ${Ogre_LIB_DEBUG}
        "D:/Projects/Radakan/ode_0_8/lib/debuglib" 
        "E:/Projects/ScatteredLands/ode-0.8/lib/debuglib"        
        "C:/ode/lib/releaselib")

else(WIN32)
    find_path(Ode_INCLUDE_DIR "ode.h"
        "/usr/local/include/ode")

    find_path(Ode_LIB_RELEASE "libode.so" 
        "/usr/local/lib")

    find_path(Ode_LIB_DEBUG "libode.so" 
        "/usr/local/lib")
endif(WIN32)

set(Ode_FOUND 0)

if(Ode_INCLUDE_DIR AND Ode_LIB_DEBUG AND Ode_LIB_RELEASE)
	set(Ode_FOUND 1)
endif(Ode_INCLUDE_DIR AND Ode_LIB_DEBUG AND Ode_LIB_RELEASE)

if (Ode_FOUND)
   if (NOT Ode_FIND_QUIETLY)
      message(STATUS "Found Ode: ${Ode_LIBRARIES}")
   endif (NOT Ode_FIND_QUIETLY)
else (Ode_FOUND)
   if (Ode_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Ode")
   endif (Ode_FIND_REQUIRED)
endif (Ode_FOUND)

