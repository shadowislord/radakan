#builds the ois library

set(SRC_FILES    "src/OISEffect.cpp"
                 "src/OISForceFeedback.cpp"
                 "src/OISInputManager.cpp"
                 "src/OISJoyStick.cpp"
                 "src/OISKeyboard.cpp"
                 "src/OISObject.cpp")

set(INCLUDE_DIR "${TSLRPG_SOURCE_DIR}/external/ois-1.0RC1/includes")              
set(LIBS "")
set(LINK_DIRS "")

if(WIN32)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

    find_package(DirectX)       

    set(INCLUDE_DIR ${INCLUDE_DIR} "${TSLRPG_SOURCE_DIR}/external/ois-1.0RC1/includes/win32" ${DirectX_INCLUDE_DIR})   

    set(LIBS ${LIBS} dxguid dinput8)

    set(LINK_DIRS ${LINK_DIRS} ${DirectX_LIB_DIR})

    set(SRC_FILES   ${SRC_FILES}
                    "src/win32/Win32ForceFeedBack.cpp"
                    "src/win32/Win32InputManager.cpp"
                    "src/win32/Win32JoyStick.cpp"
                    "src/win32/Win32KeyBoard.cpp"
                    "src/win32/Win32Mouse.cpp")
                    
endif(WIN32)

include_directories( ${INCLUDE_DIR} )
link_directories( ${LINK_DIRS} )
add_library( ois ${SRC_FILES} )
target_link_libraries( ois ${LIBS})  


