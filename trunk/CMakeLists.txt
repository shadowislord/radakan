cmake_minimum_required(VERSION 2.0)
project(RADAKAN)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RADAKAN_SOURCE_DIR}/cmake)

set(none "none" STRING)
set(Audiere "Audiere" STRING)
set(Bullet "Bullet" STRING)
set(CEGUI "CEGUI" STRING)
set(OpenGUI "OpenGUI" STRING)
set(OgreOde "OgreOde" STRING)
set(PAL "PAL" STRING)

set(GUI_MODE "CEGUI" CACHE STRING "'CEGUI' (default), 'OpenGUI' or 'none'")
message (STATUS "GUI mode: ${GUI_MODE}")

set(PHYSICS_MODE "OgreOde" CACHE STRING "'OgreOde' (default), 'Bullet' or 'PAL'")
message (STATUS "Physics mode: ${PHYSICS_MODE}")

set(SOUND_MODE "Audiere" CACHE STRING "'Audiere' (default) or 'none'")
message (STATUS "Sound mode: ${SOUND_MODE}")

set ( SUBDIRS "external/ois-1.0" "src" )

if("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	set ( SUBDIRS ${SUBDIRS} "external/ogreaddons/ogreode" )
else("${PHYSICS_MODE}" STREQUAL "${OgreOde}")
	if("${PHYSICS_MODE}" STREQUAL "${PAL}")
		set ( SUBDIRS ${SUBDIRS} "external/pal" )
	endif("${PHYSICS_MODE}" STREQUAL "${PAL}")
endif("${PHYSICS_MODE}" STREQUAL "${OgreOde}")

if("${SOUND_MODE}" STREQUAL "${Audiere}")
	set ( SUBDIRS ${SUBDIRS} "external/audiere" )
endif("${SOUND_MODE}" STREQUAL "${Audiere}")

subdirs( ${SUBDIRS} )

install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
install( DIRECTORY ${RADAKAN_SOURCE_DIR}/data DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

if(WIN32)
    file(GLOB Ogre_DLL_RELEASE  "${Ogre_BIN_DIR}/release/*.dll")
    file(GLOB Ogre_DLL_DEBUG  "${Ogre_BIN_DIR}/debug/*.dll")

    install( FILES  ${Ogre_DLL_DEBUG}  DESTINATION ${RADAKAN_SOURCE_DIR}/bin/debug CONFIGURATIONS Debug )
    install( FILES  ${Ogre_DLL_RELEASE} DESTINATION ${RADAKAN_SOURCE_DIR}/bin/release CONFIGURATIONS Release )

endif(WIN32)
