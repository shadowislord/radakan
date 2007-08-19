cmake_minimum_required(VERSION 2.0)
project(RADAKAN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RADAKAN_SOURCE_DIR}/cmake)

option( USE_FMOD "Toggle usage of Fmod On / Off " )

if(NOT USE_FMOD)
    subdirs(    "external/ois-1.0RC1" 
                "external/ogreaddons/ogreode" 
                "external/audiere" 
                "src" )
else( NOT USE_FMOD )
    subdirs(    "external/ois-1.0RC1" 
                "external/ogreaddons/ogreode" 
                "src" )
endif(NOT USE_FMOD)


install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

if(WIN32)
    file(GLOB Ogre_DLL_RELEASE  "${Ogre_BIN_DIR}/release/*.dll")
    file(GLOB Ogre_DLL_DEBUG  "${Ogre_BIN_DIR}/debug/*.dll")

    install( FILES  ${Ogre_DLL_DEBUG}  DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
    install( FILES  ${Ogre_DLL_RELEASE} DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

    if(USE_FMOD)
        install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
        install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
    endif(USE_FMOD)  
endif(WIN32)

