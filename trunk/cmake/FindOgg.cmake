# - Test for Ogg
# Once loaded this will define
#   Ogg_FOUND        - system has Ogg
#   Ogg_INCLUDE_DIR  - include directory for Ogg
#   Ogg_LIB_RELEASE  - release library directory
#   Ogg_LIB_DEBUG    - release library directory

if(WIN32)
    find_path(Ogg_INCLUDE_DIR "ogg/Ogg.h"
        "D:/ScatteredLands/ogg/include"
        "D:/Projects/ScatteredLands/ogg/include"
	    "C:/ogg/include")

    find_path(Ogg_LIB_RELEASE "ogg.lib"
        "D:/ScatteredLands/ogg/lib/releaselib"
        "D:/Projects/ScatteredLands/ogg/lib/releaselib"
        "C:/ogg/lib/releaselib")

    find_path(Ogg_LIB_DEBUG "ogg.lib"
        "D:/ScatteredLands/ogg/lib/debuglib"
        "D:/Projects/ScatteredLands/ogg/lib/debuglib"
        "C:/ogg/lib/releaselib")

    set(Ogg_FOUND 0)

    if(Ogg_INCLUDE_DIR AND Ogg_LIB_DEBUG AND Ogg_LIB_RELEASE)
        set(Ogg_FOUND 1)
    endif(Ogg_INCLUDE_DIR AND Ogg_LIB_DEBUG AND Ogg_LIB_RELEASE)
else(WIN32)
    find_path(Ogg_INCLUDE_DIR "ogg.h"
        "/usr/include/ogg")

    find_path(Ogg_LIB_RELEASE "libogg.so"
        "/usr/lib")

    find_path(Ogg_LIB_DEBUG "libogg.so"
        "/usr/lib")

    set(Ogg_FOUND 0)

    if(Ogg_INCLUDE_DIR AND Ogg_LIB_DEBUG AND Ogg_LIB_RELEASE)
        set(Ogg_FOUND 1)
    endif(Ogg_INCLUDE_DIR AND Ogg_LIB_DEBUG AND Ogg_LIB_RELEASE)
endif(WIN32)

if (Ogg_FOUND)
   if (NOT Ogg_FIND_QUIETLY)
      message(STATUS "Found Ogg: ${Ogg_LIBRARIES}")
   endif (NOT Ogg_FIND_QUIETLY)
else (Ogg_FOUND)
   if (Ogg_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Ogg")
   endif (Ogg_FIND_REQUIRED)
endif (Ogg_FOUND)

