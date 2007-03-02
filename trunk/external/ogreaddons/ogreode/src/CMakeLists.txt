#build prefab library

find_package(Ogre)

file(GLOB_RECURSE SRC_FILES "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/src" *.cpp)

set(INCLUDE_DIRS    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${TSLRPG_SOURCE_DIR}/external/ode-0.8/include"
                    ${Ogre_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} )
add_library( ogreode_core SHARED ${SRC_FILES} )

set(DEBUG_LIBS      ${Ogre_DEBUG_LIB} ode)
set(RELEASE_LIBS    ${Ogre_RELEASE_LIB} ode)

target_link_libraries( ogreode_core optimized   ${RELEASE_LIBS}
                                    debug       ${DEBUG_LIBS} )
if(WIN32)
    set_target_properties( ogreode_core PROPERTIES DEFINE_SYMBOL "OGREODE_EXPORTS;_USRDL;_PRECOMP" )
endif(WIN32)

install( TARGETS ogreode_core DESTINATION ${TSLRPG_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS ogreode_core DESTINATION ${TSLRPG_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
