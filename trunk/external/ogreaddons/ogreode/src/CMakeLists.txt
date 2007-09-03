#build prefab library

find_package(Ogre)

set(SRC_FILES 	"OgreOdePrecompiledHeaders.cpp"
		"OgreOdeBody.cpp"
		"OgreOdeCollision.cpp"
		"OgreOdeDebugContact.cpp"
		"OgreOdeDebugObject.cpp"
		"OgreOdeEigenSolver.cpp"
		"OgreOdeEntityInformer.cpp"
		"OgreOdeGeometry.cpp"
		"OgreOdeJoint.cpp"
		"OgreOdeMaintainedList.cpp"
		"OgreOdeMass.cpp"		
		"OgreOdeSpace.cpp"
		"OgreOdeStepper.cpp"
		"OgreOdeUtility.cpp"
		"OgreOdeWorld.cpp" )

set(INCLUDE_DIRS   ${Ogre_INCLUDE_DIR}
		   "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
                    )            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} )
add_library( ogreode_core SHARED ${SRC_FILES} )

set(DEBUG_LIBS      ${Ogre_DEBUG_LIB} ode Ogremain_d)
set(RELEASE_LIBS    ${Ogre_RELEASE_LIB} ode Ogremain)

target_link_libraries( ogreode_core optimized   ${RELEASE_LIBS}
                                    debug       ${DEBUG_LIBS} )
if(WIN32)
    set_target_properties( ogreode_core PROPERTIES DEFINE_SYMBOL OGREODE_EXPORTS )
endif(WIN32)

install( TARGETS ogreode_core DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS ogreode_core DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
