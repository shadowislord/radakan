#build exe radakan

find_package(Boost)
find_package(Ogre)

if("${GUI_MODE}" STREQUAL "${CEGUI}")
	find_package(CEGUI)
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	find_package(Ode)
endif("${PHYSICS_MODE}" STREQUAL "${OgreOde}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	find_package(Ogg)
	find_package(Vorbis)
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

if(WIN32)
    find_package(DIrectX)
endif(WIN32)

file(GLOB_RECURSE SRC_FILES "${RADAKAN_SOURCE_DIR}/src/cpp" *.cpp)

set(INCLUDE_DIRS    "${RADAKAN_SOURCE_DIR}/src/hpp"
					${Boost_INCLUDE_DIR}
					"${RADAKAN_SOURCE_DIR}/external/tinyxml-2.5.3"
					"${RADAKAN_SOURCE_DIR}/external/ois-1.0/includes"
					${Ogre_INCLUDE_DIR}
					"${Ogre_SAMPLES_DIR}/include")

if("${GUI_MODE}" STREQUAL "${CEGUI}")
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${CEGUI_INCLUDE_DIR}
    	${CEGUI_INCLUDE_DIR}/CEGUI
    	"${OGRE_CEGUI_RENDERER_INCLUDE}")
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Ode_INCLUDE_DIR}
		"${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
		"${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include")
else("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	if("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(INCLUDE_DIRS ${INCLUDE_DIRS} "${RADAKAN_SOURCE_DIR}/external/bullet-2.66/src")
	else("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(INCLUDE_DIRS ${INCLUDE_DIRS}  ${Ode_INCLUDE_DIR}
			"${RADAKAN_SOURCE_DIR}/external/pal/pal")
	endif("${PHYSICS_MODE}" STREQUAL "${Bullet}")
endif("${PHYSICS_MODE}" STREQUAL "${OgreOde}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(INCLUDE_DIRS
		${INCLUDE_DIRS}
		${Ogg_INCLUDE_DIR}
		${Vorbis_INCLUDE_DIR}
		"${RADAKAN_SOURCE_DIR}/external/audiere/src")
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

include_directories( ${INCLUDE_DIRS} )

set(LINK_DIRS       ${Ogre_LIB_RELEASE}
                    ${Ogre_LIB_DEBUG}
                    "${Ogre_SAMPLES_DIR}/lib"
                    "${RADAKAN_BINARY_DIR}/lib")

if("${GUI_MODE}" STREQUAL "${CEGUI}")
    set(LINK_DIRS ${LINK_DIRS} ${CEGUI_LIB_RELEASE} ${CEGUI_LIB_DEBUG})
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(LINK_DIRS ${LINK_DIRS}
					${Audiere_LIB_RELEASE}
					${Audiere_LIB_DEBUG}
					${Ogg_LIB_RELEASE}
					${Ogg_LIB_DEBUG}
					${Vorbis_LIB_RELEASE}
					${Vorbis_LIB_DEBUG})
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

if(WIN32)
    set(LINK_DIRS ${LINK_DIRS} ${DirectX_LIB_DIR})
    link_directories(  ${LINK_DIRS}  ${Directx_INCLUDE_DIR} )
    add_executable( radakan WIN32 ${SRC_FILES} )
else(WIN32)
    link_directories(  ${LINK_DIRS} )
    add_executable( radakan ${SRC_FILES} )
endif(WIN32)

if(WIN32)
	set(DEBUG_LIBS debug OgreMain_d
		debug tinyxml
		debug ois)
else(WIN32)
	set(DEBUG_LIBS debug OgreMain
		debug tinyxml
		debug ois)
endif(WIN32)

set(RELEASE_LIBS
	optimized OgreMain
	optimized tinyxml
	optimized ois)

if("${GUI_MODE}" STREQUAL "${CEGUI}")
	if(WIN32)
		set(DEBUG_LIBS ${DEBUG_LIBS}
			debug CEGUIBase_d
			debug OgreGUIRenderer_d)
		set(RELEASE_LIBS ${RELEASE_LIBS}
			optimized CEGUIBase
			optimized OgreGUIRenderer)
	else(WIN32)
		set(DEBUG_LIBS ${DEBUG_LIBS}
			debug CEGUIBase
			debug CEGUIOgreRenderer)
		set(RELEASE_LIBS ${RELEASE_LIBS}
			optimized CEGUIBase
			optimized CEGUIOgreRenderer)
	endif(WIN32)
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	set(DEBUG_LIBS ${DEBUG_LIBS} debug ode
		debug prefab
		debug ogreode_core )
	set(RELEASE_LIBS ${RELEASE_LIBS} optimized ode
		optimized prefab
		optimized ogreode_core )
else("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	if("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(DEBUG_LIBS ${DEBUG_LIBS} debug LibBulletCollision
			debug LibBulletDynamics
			debug LibLinearMath )
		set(RELEASE_LIBS ${RELEASE_LIBS} optimized LibBulletCollision
			optimized LibBulletDynamics
			optimized LibLinearMath )
	else("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(DEBUG_LIBS ${DEBUG_LIBS} debug ode
			debug prefab
			debug pal )
		set(RELEASE_LIBS ${RELEASE_LIBS} optimized ode
			optimized prefab
			optimized pal )
	endif("${PHYSICS_MODE}" STREQUAL "${Bullet}")
endif("${PHYSICS_MODE}" STREQUAL "${OgreOde}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(DEBUG_LIBS ${DEBUG_LIBS}
		debug audiere
		debug ogg
		debug vorbis)
	set(RELEASE_LIBS ${RELEASE_LIBS}
		optimized audiere
		optimized ogg
		optimized vorbis )
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

target_link_libraries( radakan  ${DEBUG_LIBS}
                                ${RELEASE_LIBS} )

set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_AUDIERE_MODE=3 -DRADAKAN_CEGUI_MODE=4 -DRADAKAN_OPENGUI_MODE=5 -DRADAKAN_OGREODE_MODE=6 -DRADAKAN_BULLET_MODE=7 -DRADAKAN_PAL_MODE=8")

if(UNIX)
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -Wall -Wextra")
endif(UNIX)

if("${GUI_MODE}" STREQUAL "${CEGUI}")
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_GUI_MODE=4")
else("${GUI_MODE}" STREQUAL "${CEGUI}")
	if("${GUI_MODE}" STREQUAL "${OpenGUI}")
		set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_GUI_MODE=5")
	endif("${GUI_MODE}" STREQUAL "${OpenGUI}")
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_PHYSICS_MODE=6")
else("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	if("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_PHYSICS_MODE=7")
	else("${PHYSICS_MODE}" STREQUAL "${Bullet}")
		set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_PHYSICS_MODE=8")
	endif("${PHYSICS_MODE}" STREQUAL "${Bullet}")
endif("${PHYSICS_MODE}" STREQUAL "${OgreOde}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_AUDIO_MODE=3")
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

set_target_properties(radakan PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")

install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
