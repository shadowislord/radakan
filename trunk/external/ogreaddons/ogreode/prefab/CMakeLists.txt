#build prefab library

find_package(Ogre)

set(SRC_FILES   "src/OgreOdeObject.cpp"
                "src/OgreOdePrefabObject.cpp"
                "src/OgreOdeRagdoll.cpp"
                "src/OgreOdeVehicle.cpp")

set(INCLUDE_DIRS    ${Ogre_INCLUDE_DIR}
					"${RADAKAN_SOURCE_DIR}/external/ode-0.9/include"
					"${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
                    )            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE}
                    ${Ogre_LIB_DEBUG} 
					"${OGRE_HOME}/Dependencies/Lib/Debug"
					"${OGRE_HOME}/Dependencies/Lib/Release"
                    ${RADAKAN_BINARY_DIR}/ogreode_core
                    ${RADAKAN_BINARY_DIR}/prefab
                    ${RADAKAN_BINARY_DIR}/tinyxml)
add_library( prefab SHARED ${SRC_FILES} )

if(WIN32)
	set(DEBUG_LIBS      OgreMain_d ogreode_core tinyxml)
else(WIN32)
	set(DEBUG_LIBS      OgreMain ogreode_core tinyxml)
endif(WIN32)
set(RELEASE_LIBS    OgreMain ogreode_core tinyxml)

target_link_libraries( prefab 	optimized ${RELEASE_LIBS}
       				debug ${DEBUG_LIBS})

if(WIN32)
    set_target_properties( prefab PROPERTIES DEFINE_SYMBOL OGREODEEXPORT_PREFAB )
endif(WIN32)

install( TARGETS prefab DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS prefab DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
