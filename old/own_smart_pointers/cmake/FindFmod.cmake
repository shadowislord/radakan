# - Test for Fmod
# Once loaded this will define
#   Fmod_FOUND        - system has Fmod
#   Fmod_INCLUDE_DIR  - include directory for Fmod
#   Fmod_LIB_RELEASE  - release libraries directory
#   Fmod_LIB_DEBUG    - debug libraries directory
#   Fmod_BIN_DIR      - binary directory  

set(Fmod_FOUND "NO")

if(WIN32)
    find_path(Fmod_INCLUDE_DIR "fmod.h"
		"D:/Projects/Radakn/fmodapi375win/api/inc"
        "D:/ScatteredLands/fmodapi375win/api/inc"
        "D:/Projects/ScatteredLands/fmodapi375win/api/inc"
        "C:/Fmod/api/inc")

    find_path(Fmod_LIB_RELEASE "fmodvc.lib" 
		"D:/Projects/Radakn/fmodapi375win/api/lib"
        "D:/ScatteredLands/fmodapi375win/api/lib"
        "D:/Projects/ScatteredLands/fmodapi375win/api/lib"
        "C:/Fmod/api/lib")

    find_path(Fmod_LIB_DEBUG "fmodvc.lib" 
		"D:/Projects/Radakn/fmodapi375win/api/lib"
        "D:/ScatteredLands/fmodapi375win/api/lib"
        "D:/Projects/ScatteredLands/fmodapi375win/api/lib"
        "C:/Fmod/api/lib")

    find_path(Fmod_BIN_DIR "fmod.dll" 
		"D:/Projects/Radakn/fmodapi375win/api"
        "D:/ScatteredLands/fmodapi375win/api"
        "D:/Projects/ScatteredLands/fmodapi375win/api"
        "C:/Fmod/apib")

    if(Fmod_INCLUDE_DIR AND Fmod_LIB_RELEASE AND Fmod_LIB_DEBUG)
        set(Fmod_FOUND "YES")
    endif(Fmod_INCLUDE_DIR AND Fmod_LIB_RELEASE AND Fmod_LIB_DEBUG)
else(WIN32)
endif(WIN32)

if (Fmod_FOUND)
   if (NOT Fmod_FIND_QUIETLY)
      message(STATUS "Found Fmod: ${Fmod_LIBRARIES}")
   endif (NOT Fmod_FIND_QUIETLY)
else (Fmod_FOUND)
   if (Fmod_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Fmod")
   endif (Fmod_FIND_REQUIRED)
endif (Fmod_FOUND)
