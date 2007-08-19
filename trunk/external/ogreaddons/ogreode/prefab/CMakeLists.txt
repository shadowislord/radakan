#build prefab library

find_package(Ogre)
find_package(Ode)

set(SRC_FILES   "src/OgreOdeObject.cpp"
                "src/OgreOdePrefabObject.cpp"
                "src/OgreOdeRagdoll.cpp"
                "src/OgreOdeVehicle.cpp")

set(INCLUDE_DIRS    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${RADAKAN_SOURCE_DIR}/external/ode-0.8/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    ${Ogre_INCLUDE_DIR})            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} 
                    ${RADAKAN_BINARY_DIR}/ogreode_core
                    ${RADAKAN_BINARY_DIR}/prefab
                    ${RADAKAN_BINARY_DIR}/tinyxml)
add_library( prefab SHARED ${SRC_FILES} )

#	set(DEBUG_LIBS      OgreMain ogreode_core tinyxml)
#	set(RELEASE_LIBS    OgreMain ogreode_core tinyxml)
target_link_libraries( prefab OgreMain ogreode_core tinyxml)

if(WIN32)
    set_target_properties( prefab PROPERTIES DEFINE_SYMBOL "OGREODEEXPORT_PREFAB;_USRDL;_PRECOMP" )
endif(WIN32)

install( TARGETS prefab DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS prefab DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
