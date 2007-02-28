# - Test for Vorbis
# Once loaded this will define
#   Vorbis_FOUND        - system has Vorbis
#   Vorbis_INCLUDE_DIR  - include directory for Vorbis
#   Vorbis_LIB_RELEASE  - release library directory
#   Vorbis_LIB_DEBUG    - release library directory

find_package(Ogre)

if(WIN32)
    find_path(Vorbis_INCLUDE_DIR "vorbis/Vorbis.h"
        "D:/ScatteredLands/vorbis/include"
        "E:/Projects/ScatteredLands/vorbis/include"
	    "C:/vorbis/include")

    find_path(Vorbis_LIB_RELEASE "vorbis.lib"
        "D:/ScatteredLands/vorbis/lib/releaselib"
        "E:/Projects/ScatteredLands/vorbis/lib/releaselib"
        "C:/vorbis/lib/releaselib")

    find_path(Vorbis_LIB_DEBUG "vorbis.lib"
        "D:/ScatteredLands/vorbis/lib/debuglib"
        "E:/Projects/ScatteredLands/vorbis/lib/debuglib"
        "C:/vorbis/lib/releaselib")

    set(Vorbis_FOUND 0)

    if(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)
        set(Vorbis_FOUND 1)
    endif(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)
else(WIN32)
    find_path(Vorbis_INCLUDE_DIR "vorbisenc.h"
        "/usr/include/vorbis")

    find_path(Vorbis_LIB_RELEASE "libvorbis.so"
        "/usr/lib")

    find_path(Vorbis_LIB_DEBUG "libvorbis.so"
        "/usr/lib")

    set(Vorbis_FOUND 0)

    if(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)
        set(Vorbis_FOUND 1)
    endif(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)
endif(WIN32)

if (Vorbis_FOUND)
   if (NOT Vorbis_FIND_QUIETLY)
      message(STATUS "Found Vorbis: ${Vorbis_LIBRARIES}")
   endif (NOT Vorbis_FIND_QUIETLY)
else (Vorbis_FOUND)
   if (Vorbis_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Vorbis")
   endif (Vorbis_FIND_REQUIRED)
endif (Vorbis_FOUND)

