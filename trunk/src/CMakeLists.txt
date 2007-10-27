#build exe radakan

find_package(Boost)
find_package(Ogre)
find_package(CEGUI)

if(USE_FMOD)
    find_package(Fmod)
else(USE_FMOD)
   find_package(Ogg)
   find_package(Vorbis)
endif(USE_FMOD)

if(WIN32)
    find_package(DIrectX)
endif(WIN32)

file(GLOB_RECURSE SRC_FILES "${RADAKAN_SOURCE_DIR}/src/cpp" *.cpp)

set(INCLUDE_DIRS    "${RADAKAN_SOURCE_DIR}/src/hpp"
                    ${Boost_INCLUDE_DIR}
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    "${RADAKAN_SOURCE_DIR}/external/tinyxml-2.5.2"
                    "${RADAKAN_SOURCE_DIR}/external/ois-1.0RC1/includes"
                    "${RADAKAN_SOURCE_DIR}/external/audiere/src"
                    ${Ogre_INCLUDE_DIR}
                    "${Ogre_SAMPLES_DIR}/include"
                    ${Ogg_INCLUDE_DIR}
                    ${Vorbis_INCLUDE_DIR}
                    ${CEGUI_INCLUDE_DIR})
if(USE_FMOD)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Fmod_INCLUDE_DIR})
endif(USE_FMOD)

set(LINK_DIRS       ${Ogre_LIB_RELEASE}
                    ${Ogre_LIB_DEBUG}
                    ${CEGUI_LIB_RELEASE}
                    ${CEGUI_LIB_DEBUG}
                    "${Ogre_SAMPLES_DIR}/lib"
                    "${RADAKAN_BINARY_DIR}/lib")

if(USE_FMOD)
    set(LINK_DIRS ${INCLUDE_DIRS}  ${Fmod_LIB_RELEASE})
else(USE_FMOD)
    set(LINK_DIRS ${LINK_DIRS}
                    ${Audiere_LIB_RELEASE}
                    ${Audiere_LIB_DEBUG}
                    ${Ogg_LIB_RELEASE}
                    ${Ogg_LIB_DEBUG}
                    ${Vorbis_LIB_RELEASE}
                    ${Vorbis_LIB_DEBUG})
endif(USE_FMOD)

if(WIN32)
    set(LINK_DIRS ${LINK_DIRS} ${DirectX_LIB_DIR})
    link_directories(  ${LINK_DIRS}  ${Directx_INCLUDE_DIR} )
    add_executable( radakan WIN32 ${SRC_FILES} )
else(WIN32)
    link_directories(  ${LINK_DIRS} )
    add_executable( radakan ${SRC_FILES} )
endif(WIN32)

include_directories( ${INCLUDE_DIRS} )


if(WIN32)
    set(DEBUG_LIBS  debug OgreMain_d
                debug CEGUIBase_d
                debug OgreGUIRenderer_d
                debug tinyxml
                debug prefab
                debug ogreode_core
                debug ois)
    set(RELEASE_LIBS  optimized OgreMain
                    optimized CEGUIBase
                    optimized OgreGUIRenderer
                    optimized tinyxml
                    optimized prefab
                    optimized ogreode_core
                    optimized ois)
else(WIN32)
    set(DEBUG_LIBS  debug OgreMain
                debug CEGUIBase
                debug CEGUIOgreRenderer
                debug tinyxml
                debug prefab
                debug ogreode_core
                debug ois)
    set(RELEASE_LIBS  optimized OgreMain
                optimized CEGUIBase
                optimized CEGUIOgreRenderer
                optimized tinyxml
                optimized prefab
                optimized ogreode_core
                optimized ois)
endif(WIN32)
if(USE_FMOD)
    if(WIN32)
        set(DEBUG_LIBS ${DEBUG_LIBS}  fmodvc)
        set(RELEASE_LIBS ${RELEASE_LIBS}  fmodvc)
    else(WIN32)
    endif(WIN32)
else(USE_FMOD)
    set(DEBUG_LIBS ${DEBUG_LIBS}
                debug audiere
                debug ogg
                debug vorbis)
    set(RELEASE_LIBS ${RELEASE_LIBS}
                optimized audiere
                optimized ogg
                optimized vorbis )
endif(USE_FMOD)

target_link_libraries( radakan  ${DEBUG_LIBS}
                                ${RELEASE_LIBS} )

install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
