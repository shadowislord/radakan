# - Test for DirectX
# Once loaded this will define
#   DirectX_FOUND        - system has DirectX
#   DirectX_INCLUDE_DIR  - include directory for DirectX
#   DirectX_LIB_DIR      - lib directory for DirectX

set(DirectX_FOUND "NO")

if(WIN32)
    find_path(DirectX_INCLUDE_DIR "dinput.h"
        "C:/Program Files/Microsoft DirectX SDK (February 2007)/Include"
        "C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Include"
        "C:/Program Files/Microsoft DirectX SDK (February 2006)/Include"
        "C:/Program Files/Microsoft DirectX 9.0 SDK (June 2005)/Include"
        "C:/DXSDK/Include")

    find_path(DirectX_LIB_DIR "dinput8.lib"
        "C:/Program Files/Microsoft DirectX SDK (February 2007)/Lib/x86"
        "C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Lib"
        "C:/Program Files/Microsoft DirectX SDK (February 2006)/Lib"
        "C:/Program Files/Microsoft DirectX 9.0 SDK (June 2005)/Lib"
        "C:/DXSDK/Lib")

    if(DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)
        set(DirectX_FOUND "YES")
    endif(DirectX_INCLUDE_DIR AND DirectX_LIB_DIR)
else(WIN32)
endif(WIN32)

if (DirectX_FOUND)
   if (NOT DirectX_FIND_QUIETLY)
      message(STATUS "Found DirectX: ${DirectX_LIBRARIES}")
   endif (NOT DirectX_FIND_QUIETLY)
else (DirectX_FOUND)
   if (DirectX_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find DirectX")
   endif (DirectX_FIND_REQUIRED)
endif (DirectX_FOUND)
