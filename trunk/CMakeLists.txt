cmake_minimum_required(VERSION 2.0)
project(RADAKAN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RADAKAN_SOURCE_DIR}/cmake)

set(none "none" STRING)
set(CEGUI "CEGUI" STRING)
set(OpenGUI "OpenGUI" STRING)
set(Audiere "Audiere" STRING)
set(FMod "FMod" STRING)

set(GUI_MODE "CEGUI" CACHE STRING "'CEGUI' (default), 'OpenGUI' or 'none'")
message (STATUS "GUI mode: ${GUI_MODE}")

set(SOUND_MODE "Audiere" CACHE STRING "'Audiere' (default), 'FMod' or 'none'")
message (STATUS "Sound mode: ${SOUND_MODE}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
    subdirs(    "external/ois-1.0"
                "external/ogreaddons/ogreode"
                "external/audiere"
                "src" )
else("${SOUND_MODE}" STREQUAL "${Audiere}")
    subdirs(    "external/ois-1.0"
                "external/ogreaddons/ogreode"
                "src" )
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

if(WIN32)
    file(GLOB Ogre_DLL_RELEASE  "${Ogre_BIN_DIR}/release/*.dll")
    file(GLOB Ogre_DLL_DEBUG  "${Ogre_BIN_DIR}/debug/*.dll")

    install( FILES  ${Ogre_DLL_DEBUG}  DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
    install( FILES  ${Ogre_DLL_RELEASE} DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

    if("${SOUND_MODE}" STREQUAL "${FMod}")
        install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
        install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
    endif("${SOUND_MODE}" STREQUAL "${FMod}")
endif(WIN32)
