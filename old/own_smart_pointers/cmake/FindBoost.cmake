# - Test for Boost
# Once loaded this will define
#   Boost_FOUND        - system has Boost
#   Boost_INCLUDE_DIR  - include directory for Boost

set(Boost_FOUND "NO")

if(WIN32)
    find_path(Boost_INCLUDE_DIR "boost/regex.h"
		"D:/claudiu/projects/Radakan/boost_1_34_1"
		"D:/Projects/Radakan/Boost/include/boost-1_33_1"
        "D:/ScatteredLands/Boost/include/boost-1_33_1"
        "D:/ScatteredLands/Boost/include/boost-1_33_1" 
        "D:/Projects/ScatteredLands/boost/include/boost-1_33_1" 
        "C:/Boost/includeboost-1_33_1")

else(WIN32)
    find_path(Boost_INCLUDE_DIR "boost/regex.h"
        "/usr/include")
endif(WIN32)

if(Boost_INCLUDE_DIR)
    set(Boost_FOUND "YES")
endif(Boost_INCLUDE_DIR)

if (Boost_FOUND)
   if (NOT Boost_FIND_QUIETLY)
      message(STATUS "Found Boost: ${Boost_INCLUDE_DIR}")
   endif (NOT Boost_FIND_QUIETLY)
else (Boost_FOUND)
   if (Boost_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Boost")
   else (Boost_FIND_REQUIRED)
      message(STATUS "Could not find Boost")
   endif (Boost_FIND_REQUIRED)
endif (Boost_FOUND)
