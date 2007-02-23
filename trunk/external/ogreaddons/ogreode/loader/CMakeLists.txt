#build loader library

find_package(Ogre)

set(SRC_FILES   "src/OgreOdeDotLoader.cpp")

set(INCLUDE_DIRS    "${TSLRPG_SOURCE_DIR}/external/ode-0.8/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/loader/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/tinyxml-2.3.3"
                    ${Ogre_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} 
                    ${TSLRPG_BINARY_DIR}/odeogre_core
                    ${TSLRPG_BINARY_DIR}/tinyxml)
add_library( loader SHARED ${SRC_FILES} )

set(DEBUG_LIBS      ogremain_d tinyxml prefab ogreode_core)
set(RELEASE_LIBS    ogremain tinyxml prefab ogreode_core)
target_link_libraries( loader   optimized   ${RELEASE_LIBS}
                                debug       ${DEBUG_LIBS} )

if(WIN32)
    set_target_properties( loader PROPERTIES    DEFINE_SYMBOL "OGREODEEXPORT_Loader;_USRDL;_PRECOMP" )
endif(WIN32) 

install( TARGETS loader DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS loader DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
