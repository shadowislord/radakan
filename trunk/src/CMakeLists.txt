#build exe radakan

find_package(Boost)
find_package(Ogre)

if("${GUI_MODE}" STREQUAL "${CEGUI}")
	find_package(CEGUI)
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	find_package(Ogg)
	find_package(Vorbis)
else("${SOUND_MODE}" STREQUAL "${Audiere}")
	if("${SOUND_MODE}" STREQUAL "${FMod}")
		find_package(Fmod)
	endif("${SOUND_MODE}" STREQUAL "${FMod}")
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

if(WIN32)
    find_package(DIrectX)
endif(WIN32)

file(GLOB_RECURSE SRC_FILES "${RADAKAN_SOURCE_DIR}/src/cpp" *.cpp)

set(INCLUDE_DIRS    "${RADAKAN_SOURCE_DIR}/src/hpp"
					${Boost_INCLUDE_DIR}
					"${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/include"
					"${RADAKAN_SOURCE_DIR}/external/ogreaddons/ogreode/prefab/include"
					"${RADAKAN_SOURCE_DIR}/external/tinyxml-2.5.2"
					"${RADAKAN_SOURCE_DIR}/external/ois-1.0/includes"
					${Ogre_INCLUDE_DIR}
					"${Ogre_SAMPLES_DIR}/include")

if("${GUI_MODE}" STREQUAL "${CEGUI}")
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${CEGUI_INCLUDE_DIR} "${Ogre_INCLUDE_DIR}/cegui")
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(INCLUDE_DIRS
		${INCLUDE_DIRS}
		${Ogg_INCLUDE_DIR}
		${Vorbis_INCLUDE_DIR}
		"${RADAKAN_SOURCE_DIR}/external/audiere/src")
else("${SOUND_MODE}" STREQUAL "${Audiere}")
	if("${SOUND_MODE}" STREQUAL "${FMod}")
		set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Fmod_INCLUDE_DIR})
	endif("${SOUND_MODE}" STREQUAL "${FMod}")
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
else("${SOUND_MODE}" STREQUAL "${Audiere}")
	if("${SOUND_MODE}" STREQUAL "${FMod}")
		set(LINK_DIRS ${LINK_DIRS} ${Fmod_LIB_RELEASE})
	endif("${SOUND_MODE}" STREQUAL "${FMod}")
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
		debug prefab
		debug ogreode_core
		debug ois)
else(WIN32)
	set(DEBUG_LIBS debug OgreMain
		debug tinyxml
		debug prefab
		debug ogreode_core
		debug ois)
endif(WIN32)

set(RELEASE_LIBS
	optimized OgreMain
	optimized tinyxml
	optimized prefab
	optimized ogreode_core
	optimized ois)

if(WIN32)

	if("${GUI_MODE}" STREQUAL "${CEGUI}")
		set(DEBUG_LIBS
			${DEBUG_LIBS}
			debug CEGUIBase_d
			debug OgreGUIRenderer_d)

		set(RELEASE_LIBS
			${RELEASE_LIBS}
			optimized CEGUIBase
			optimized OgreGUIRenderer)

	endif("${GUI_MODE}" STREQUAL "${CEGUI}")
else(WIN32)				
	if("${GUI_MODE}" STREQUAL "${CEGUI}")
		set(DEBUG_LIBS
			${DEBUG_LIBS}
			debug CEGUIBase
			debug CEGUIOgreRenderer)

		set(RELEASE_LIBS
			${RELEASE_LIBS}
			optimized CEGUIBase
			optimized CEGUIOgreRenderer)

	endif("${GUI_MODE}" STREQUAL "${CEGUI}")
endif(WIN32)

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set(DEBUG_LIBS ${DEBUG_LIBS}
		debug audiere
		debug ogg
		debug vorbis)
	set(RELEASE_LIBS ${RELEASE_LIBS}
		optimized audiere
		optimized ogg
		optimized vorbis )
else("${SOUND_MODE}" STREQUAL "${Audiere}")
	if("${SOUND_MODE}" STREQUAL "${FMod}")
		if(WIN32)
			set(DEBUG_LIBS ${DEBUG_LIBS} fmodvc)
			set(RELEASE_LIBS ${RELEASE_LIBS} fmodvc)
		endif(WIN32)
	endif("${SOUND_MODE}" STREQUAL "${FMod}")
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

target_link_libraries( radakan  ${DEBUG_LIBS}
                                ${RELEASE_LIBS} )

set_target_properties(radakan PROPERTIES COMPILE_FLAGS "-DRADAKAN_CEGUI_MODE=1 -DRADAKAN_OPENGUI_MODE=2 -DRADAKAN_AUDIERE_MODE=3 -DRADAKAN_FMOD_MODE=4 -Wall -Wextra")

if("${GUI_MODE}" STREQUAL "${CEGUI}")
	set_target_properties(radakan PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_GUI_MODE=1")
else("${GUI_MODE}" STREQUAL "${CEGUI}")
	if("${GUI_MODE}" STREQUAL "${OpenGUI}")
		set_target_properties(radakan PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_GUI_MODE=2")
	endif("${GUI_MODE}" STREQUAL "${OpenGUI}")
endif("${GUI_MODE}" STREQUAL "${CEGUI}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set_target_properties(radakan PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_AUDIO_MODE=3")
else("${SOUND_MODE}" STREQUAL "${Audiere}")
	if("${SOUND_MODE}" STREQUAL "${FMod}")
		set_target_properties(radakan PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DRADAKAN_AUDIO_MODE=4")
	endif("${SOUND_MODE}" STREQUAL "${FMod}")
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( TARGETS radakan DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )
