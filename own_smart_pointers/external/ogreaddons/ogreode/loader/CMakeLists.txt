#build loader library

find_package(Ogre)
find_package(Ode)

set(SRC_FILES   "src/OgreOdeDotLoader.cpp")

set(INCLUDE_DIRS    "${RADAKAN_SOURCE_DIR}/external/ode-0.8/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/loader/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/tinyxml-2.3.3"
                    ${Ogre_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} 
                    ${RADAKAN_BINARY_DIR}/ogreode_core
                    ${RADAKAN_BINARY_DIR}/tinyxml
                    ${RADAKAN_BINARY_DIR}/prefab)
add_library( loader SHARED ${SRC_FILES} )

set(DEBUG_LIBS      OgreMain tinyxml prefab ogreode_core)
set(RELEASE_LIBS    OgreMain tinyxml prefab ogreode_core)
target_link_libraries( loader   optimized   ${RELEASE_LIBS}
                                debug       ${DEBUG_LIBS} )

if(WIN32)
    set_target_properties( loader PROPERTIES    DEFINE_SYMBOL "OGREODEEXPORT_LOADER;_USRDL;_PRECOMP" )
endif(WIN32) 

install( TARGETS loader DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS loader DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
