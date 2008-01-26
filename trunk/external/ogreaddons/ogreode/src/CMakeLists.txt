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

set(INCLUDE_DIRS   	${Ogre_INCLUDE_DIR}
					"${RADAKAN_SOURCE_DIR}/external/ode-0.9/include"
		            "${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include" )            

include_directories( ${INCLUDE_DIRS} )                
link_directories(   ${Ogre_LIB_RELEASE} ${Ogre_LIB_DEBUG})
add_library( ogreode_core SHARED ${SRC_FILES} )

if(WIN32)
    set(DEBUG_LIBS      debug ode 
    	   		    debug OgreMain_d)
else(WIN32)
    set(DEBUG_LIBS      debug ode 
    	   		    debug OgreMain)
endif(WIN32)
set(RELEASE_LIBS    optimized ode
					optimized OgreMain)

target_link_libraries( ogreode_core ${RELEASE_LIBS}
                                    ${DEBUG_LIBS} )
if(WIN32)
    set_target_properties( ogreode_core PROPERTIES DEFINE_SYMBOL OGREODE_EXPORTS )
endif(WIN32)

install( TARGETS ogreode_core DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS ogreode_core DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
