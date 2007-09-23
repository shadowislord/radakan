# - Test for Vorbis
# Once loaded this will define
#   Vorbis_FOUND        - system has Vorbis
#   Vorbis_INCLUDE_DIR  - include directory for Vorbis
#   Vorbis_LIB_RELEASE  - release library directory
#   Vorbis_LIB_DEBUG    - release library directory

if(WIN32)
    find_path(Vorbis_INCLUDE_DIR "vorbis/Vorbis.h"
        "D:/ScatteredLands/vorbis/include"
        "D:/Projects/ScatteredLands/vorbis/include"
	    "C:/vorbis/include")

    find_path(Vorbis_LIB_RELEASE "vorbis.lib"
        "D:/ScatteredLands/vorbis/lib/releaselib"
        "D:/Projects/ScatteredLands/vorbis/lib/releaselib"
        "C:/vorbis/lib/releaselib")

    find_path(Vorbis_LIB_DEBUG "vorbis.lib"
        "D:/ScatteredLands/vorbis/lib/debuglib"
        "D:/Projects/ScatteredLands/vorbis/lib/debuglib"
        "C:/vorbis/lib/releaselib")
else(WIN32)
    find_path(Vorbis_INCLUDE_DIR "vorbisenc.h"
        "/usr/include/vorbis")

    find_path(Vorbis_LIB_RELEASE "libvorbis.so"
        "/usr/lib")

    find_path(Vorbis_LIB_DEBUG "libvorbis.so"
        "/usr/lib")
endif(WIN32)

set(Vorbis_FOUND 0)

if(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)
#    set(Vorbis_FOUND 1)
endif(Vorbis_INCLUDE_DIR AND Vorbis_LIB_DEBUG AND Vorbis_LIB_RELEASE)

if (Vorbis_FOUND)
    if (NOT Vorbis_FIND_QUIETLY)
        message (STATUS "Found Vorbis: ${Vorbis_INCLUDE_DIR}, ${Vorbis_LIB_RELEASE}, ${Vorbis_LIB_DEBUG}")
    endif (NOT Vorbis_FIND_QUIETLY)
else (Vorbis_FOUND)
    if (Vorbis_FIND_REQUIRED)
        message (FATAL_ERROR "Could not find Vorbis")
    else (Vorbis_FIND_REQUIRED)
        message (STATUS "Could not find Vorbis")
    endif (Vorbis_FIND_REQUIRED)
endif (Vorbis_FOUND)
