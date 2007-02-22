#build prefab library

find_package(Ogre)

set(SRC_FILES   "src/OgreOdeObject.cpp"
                "src/OgreOdePrefabObject.cpp"
                "src/OgreOdeRagdoll.cpp"
                "src/OgreOdeVehicle.cpp")

set(INCLUDE_DIRS    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${TSLRPG_SOURCE_DIR}/external/ode-0.8/include"
                    "${TSLRPG_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    ${Ogre_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} 
                    ${TSLRPG_BINARY_DIR}/odeogre_core
                    ${TSLRPG_BINARY_DIR}/prefab
                    ${TSLRPG_BINARY_DIR}/tinyxml)
add_library( prefab SHARED ${SRC_FILES} )

set(DEBUG_LIBS      ogremain_d ogreode_core tinyxml)
set(RELEASE_LIBS    ogremain ogreode_core tinyxml)
target_link_libraries( prefab   optimized   ${RELEASE_LIBS}
                                debug       ${DEBUG_LIBS} )
if(WIN32)
    set_target_properties( prefab PROPERTIES DEFINE_SYMBOL "OGREODEEXPORT_PREFAB;_USRDL;_PRECOMP" )
endif(WIN32)
