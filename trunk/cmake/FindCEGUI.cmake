# - Test for CEGUI
# Once loaded this will define
#   CEGUI_FOUND        - system has CEGUI
#   CEGUI_INCLUDE_DIR  - include directory for CEGUI
#   CEGUI_LIB_RELEASE  - release libraries directory
#   CEGUI_LIB_DEBUG    - debug libraries directory
#   CEGUI_BIN_DIR      - ogre binary directory 

set(CEGUI_FOUND "NO")

if(WIN32)
    if(MSVC80)
        find_path(CEGUI_INCLUDE_DIR "CEGUI.h"
            "D:/ScatteredLands/OgreSDK_1_2_5/include/CEGUI"
            "D:/ScatteredLands/OgreSDK/include/CEGUI" 
            "E:/Projects/ScatteredLands/OgreSDK/include/CEGUI" 
        	"C:/OgreSDK/include/CEGUI")

        find_path(CEGUI_BIN_DIR "release/CEGUIBase.dll"
            "D:/ScatteredLands/OgreSDK_1_2_5/bin"
            "D:/ScatteredLands/OgreSDK/bin" 
            "E:/Projects/ScatteredLands/OgreSDK/bin" 
        	"C:/OgreSDK/bin")

        find_path(CEGUI_LIB_RELEASE "CEGUIBase.lib" 
	        "D:/ScatteredLands/OgreSDK_1_2_5/lib"
            "D:/ScatteredLands/OgreSDK/lib" 
            "E:/Projects/ScatteredLands/OgreSDK/lib" 
            "C:/OgreSDK/lib")

        find_path(CEGUI_LIB_DEBUG "CEGUIBase_d.lib" 
            "D:/ScatteredLands/OgreSDK_1_2_5/lib"
            "D:/ScatteredLands/OgreSDK/lib" 
            "E:/Projects/ScatteredLands/OgreSDK/lib" 
            "C:/OgreSDK/lib")
    else(MSVC80)
        if(MINGW)
            find_path(CEGUI_INCLUDE_DIR "CEGUI.h"
                "D:/ScatteredLands/OgreSDK_GCC/include/CEGUI"
            	"C:/OgreSDK/include/CEGUI")

            find_path(CEGUI_BIN_DIR "release/CEGUIBase.dll"
                "D:/ScatteredLands/OgreSDK_GCC/bin"
            	"C:/OgreSDK/bin")

            find_path(CEGUI_LIB_RELEASE "CEGUIBase.dll" 
	            "D:/ScatteredLands/OgreSDK_GCC/bin/release"
                "C:/OgreSDK/bin/release")

            find_path(CEGUI_LIB_DEBUG "CEGUIBase_d.dll" 
                "D:/ScatteredLands/OgreSDK_GCC/bin/debug"
                "C:/OgreSDK/bin/debug")
        endif(MINGW)
    endif(MSVC80)
else(WIN32)
    find_path(CEGUI_INCLUDE_DIR "CEGUI.h"
        "/usr/include/CEGUI")

    find_path(CEGUI_LIB_RELEASE "libCEGUIBase.so"
        "/usr/lib")

    find_path(CEGUI_LIB_DEBUG "libCEGUIBase.so"
        "/usr/lib")
endif(WIN32)

if(CEGUI_INCLUDE_DIR AND CEGUI_LIB_RELEASE AND CEGUI_LIB_DEBUG)
	set(CEGUI_FOUND "YES")
endif(CEGUI_INCLUDE_DIR AND CEGUI_LIB_RELEASE AND CEGUI_LIB_DEBUG)

if (CEGUI_FOUND)
   if (NOT CEGUI_FIND_QUIETLY)
      message(STATUS "Found CEGUI: ${CEGUI_LIBRARIES}")
   endif (NOT CEGUI_FIND_QUIETLY)
else (CEGUI_FOUND)
   if (CEGUI_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find CEGUI")
   endif (CEGUI_FIND_REQUIRED)
endif (CEGUI_FOUND)

