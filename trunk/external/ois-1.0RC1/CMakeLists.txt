#builds the ois library

set(SRC_FILES    "src/OISEffect.cpp"
                 "src/OISForceFeedback.cpp"
                 "src/OISInputManager.cpp"
                 "src/OISJoyStick.cpp"
                 "src/OISKeyboard.cpp"
                 "src/OISObject.cpp")

set(INCLUDE_DIR "${RADAKAN_SOURCE_DIR}/external/ois-1.0RC1/includes")
set(LIBS "")
set(LINK_DIRS "")

if(WIN32)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

    find_package(DirectX)       

    set(INCLUDE_DIR ${INCLUDE_DIR} "${RADAKAN_SOURCE_DIR}/external/ois-1.0RC1/includes/win32" ${DirectX_INCLUDE_DIR})

    set(LIBS ${LIBS} dxguid dinput8)

    set(LINK_DIRS ${LINK_DIRS} ${DirectX_LIB_DIR})

    set(SRC_FILES   ${SRC_FILES}
                    "src/win32/Win32ForceFeedback.cpp"
                    "src/win32/Win32InputManager.cpp"
                    "src/win32/Win32Joystick.cpp"
                    "src/win32/Win32Keyboard.cpp"
                    "src/win32/Win32Mouse.cpp")

else(WIN32)
    set(SRC_FILES   ${SRC_FILES}
                    "src/linux/EventHelpers.cpp"
                    "src/linux/LinuxForceFeedback.cpp"
                    "src/linux/LinuxInputManager.cpp"
                    "src/linux/LinuxJoyStickEvents.cpp"
                    "src/linux/LinuxKeyboard.cpp"
                    "src/linux/LinuxMouse.cpp")

    set(LIBS ${LIBS} dl Xaw)
endif(WIN32)

include_directories( ${INCLUDE_DIR} )
link_directories( ${LINK_DIRS} )
add_library( ois ${SRC_FILES} )
target_link_libraries( ois ${LIBS})  

