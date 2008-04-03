# - Test for CEGUI
# Once loaded this will define
#   CEGUI_FOUND        - system has CEGUI
#   CEGUI_INCLUDE_DIR  - include directory for CEGUI
#   CEGUI_LIB_RELEASE  - release libraries directory
#   CEGUI_LIB_DEBUG    - debug libraries directory
#   CEGUI_BIN_DIR      - ogre binary directory 

set(CEGUI_FOUND "NO")
set(OGRE_HOME "$ENV{OGRE_HOME}")

if(WIN32)
    if(MSVC80)
        find_path(CEGUI_INCLUDE_DIR "CEGUI/CEGUI.h"
			"D:/Projects/Radakan/Ogre_Deps/CEGUI-0.5.0/include"
            "${OGRE_HOME}/Dependencies/include"
			"${OGRE_HOME}/include")

		find_path(OGRE_CEGUI_RENDERER_INCLUDE "OgreCEGUIRenderer.h"	
			"D:/Projects/Radakan/Ogre_Deps/CEGUI-0.5.0/include"
			"${OGRE_HOME}/Samples/Common/CEGUIRenderer/include"
			"${OGRE_HOME}/Samples/include")
			
        find_path(CEGUI_LIB_RELEASE "CEGUIBase.lib" 
            "D:/Projects/Radakan/Ogre_Deps/CEGUI-0.5.0/lib"
	        "${OGRE_HOME}/lib")

        find_path(CEGUI_LIB_DEBUG "CEGUIBase_d.lib" 
			"D:/Projects/Radakan/Ogre_Deps/CEGUI-0.5.0/lib"
            "${OGRE_HOME}/lib")
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
        message (STATUS "Found CEGUI: ${CEGUI_INCLUDE_DIR}, ${CEGUI_LIB_RELEASE}, ${CEGUI_LIB_DEBUG}")
    endif (NOT CEGUI_FIND_QUIETLY)
else (CEGUI_FOUND)
    if (CEGUI_FIND_REQUIRED)
        message (FATAL_ERROR "Could not find CEGUI")
    else (CEGUI_FIND_REQUIRED)
        message (STATUS "Could not find CEGUI")
    endif (CEGUI_FIND_REQUIRED)
endif (CEGUI_FOUND)
