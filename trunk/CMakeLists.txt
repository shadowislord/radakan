cmake_minimum_required(VERSION 2.0)
project(TSLRPG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${TSLRPG_SOURCE_DIR}/cmake)

subdirs(    "external/ois-1.0RC1" 
            "external/ode-0.8" 
            "external/ogreaddons/ogreode" 
            "external/audiere" 
            "src" )

install( DIRECTORY ${TSLRPG_SOURCE_DIR}/data DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug ) 
install( DIRECTORY ${TSLRPG_SOURCE_DIR}/data DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release ) 

if(WIN32)
    file(GLOB Ogre_DLL_RELEASE  "${Ogre_BIN_DIR}/release/*.dll")
    file(GLOB Ogre_DLL_DEBUG  "${Ogre_BIN_DIR}/debug/*.dll")

    install( FILES  ${Ogre_DLL_DEBUG}  DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug ) 
    install( FILES  ${Ogre_DLL_RELEASE} DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

    install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug ) 
    install( FILES  ${Fmod_BIN_DIR}/fmod.dll DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
    
endif(WIN32)

